/* JUnzip library by Joonas Pihlajamaa. See junzip.h for license and details. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#include "jz.h"

/* limits maximum zip descriptor size */
unsigned char jzBuffer[JZ_BUFFER_SIZE];

/**
 * jzReadEndRecord:
 * Read ZIP file end record. Will move within file.
 */
int jzReadEndRecord(JZFile *zip, JZEndRecord *endRecord)
{
	long fileSize, readBytes, i;
	JZEndRecord *er;

	if (zip->seek(zip, 0, SEEK_END)) {
		fprintf(stderr, "Couldn't go to end of zip file!\n");
		return Z_ERRNO;
	}

	fileSize = zip->tell(zip);
	if (fileSize <= sizeof(JZEndRecord)) {
		fprintf(stderr, "Too small file to be a zip!\n");
		return Z_ERRNO;
	}

	readBytes = (fileSize < sizeof(jzBuffer)) ? fileSize : sizeof(jzBuffer);

	if (zip->seek(zip, fileSize - readBytes, SEEK_SET)) {
		fprintf(stderr, "Cannot seek in zip file!\n");
		return Z_ERRNO;
	}

	if (zip->read(zip, jzBuffer, readBytes) < readBytes) {
		fprintf(stderr, "Couldn't read end of zip file!\n");
		return Z_ERRNO;
	}

	/* Naively assume signature can only be found in one place... */
	for (i = readBytes - sizeof(JZEndRecord); i >= 0; i--) {
		er = (JZEndRecord *)(jzBuffer + i);
		if (er->signature == 0x06054B50)
			break;
	}

	if (i < 0) {
		fprintf(stderr, "End record signature not found in zip!\n");
		return Z_ERRNO;
	}

	memcpy(endRecord, er, sizeof(JZEndRecord));

	if (endRecord->diskNumber || endRecord->centralDirectoryDiskNumber ||
		endRecord->numEntries != endRecord->numEntriesThisDisk) {
		fprintf(stderr, "Multifile zips not supported!\n");
		return Z_ERRNO;
	}

	return Z_OK;
}


/**
 * jzReadCentralDirectory:
 * Read ZIP file global directory. Will move within file.
 */
int jzReadCentralDirectory(JZFile *zip, JZEndRecord *endRecord,
			JZRecordCallback callback, void *user_data)
{
	JZGlobalFileHeader fileHeader;
	JZFileHeader header;
	int i;

	if (zip->seek(zip, endRecord->centralDirectoryOffset, SEEK_SET)) {
		fprintf(stderr, "Cannot seek in zip file!\n");
		return Z_ERRNO;
	}

	for (i = 0; i < endRecord->numEntries; i++) {
		if (zip->read(zip, &fileHeader, sizeof(JZGlobalFileHeader)) <
			sizeof(JZGlobalFileHeader)) {
			fprintf(stderr, "Couldn't read file header %d!\n", i);
			return Z_ERRNO;
		}

		if (fileHeader.signature != 0x02014B50) {
			fprintf(stderr,
				"Invalid file header signature %d!\n", i);
			return Z_ERRNO;
		}

		if (fileHeader.fileNameLength + 1 >= JZ_BUFFER_SIZE) {
			fprintf(stderr, "Too long file name %d!\n", i);
			return Z_ERRNO;
		}

		if (zip->read(zip, jzBuffer, fileHeader.fileNameLength) <
			fileHeader.fileNameLength) {
			fprintf(stderr, "Couldn't read filename %d!\n", i);
			return Z_ERRNO;
		}

		jzBuffer[fileHeader.fileNameLength] = '\0'; /* NULL terminate */

		if (zip->seek(zip, fileHeader.extraFieldLength, SEEK_CUR) ||
		zip->seek(zip, fileHeader.fileCommentLength, SEEK_CUR)) {
			fprintf(stderr,
				"Couldn't skip extra field or file comment %d\n",
				i);
			return Z_ERRNO;
		}

		/* Construct JZFileHeader from global file header */
		memcpy(&header, &fileHeader.compressionMethod, sizeof(header));
		header.offset = fileHeader.relativeOffsetOflocalHeader;

		if (!callback(zip, i, &header, (char *)jzBuffer, user_data))
			break; /* end if callback returns zero */
	}

	return Z_OK;
}

/**
 * jzReadLocalFileHeader:
 * Read local ZIP file header.
 * Silent on errors so optimistic reading possible.
 */
int jzReadLocalFileHeader(JZFile *zip, JZFileHeader *header,
			char *filename, int len)
{
	JZLocalFileHeader localHeader;

	if (zip->read(zip, &localHeader, sizeof(JZLocalFileHeader)) <
		sizeof(JZLocalFileHeader))
		return Z_ERRNO;

	if (localHeader.signature != 0x04034B50)
		return Z_ERRNO;

	if (len) { /* read filename */
		if (localHeader.fileNameLength >= len)
			return Z_ERRNO; /* filename cannot fit */

		if (zip->read(zip, filename, localHeader.fileNameLength) <
		localHeader.fileNameLength)
			return Z_ERRNO; /* read fail */

		/* NULL terminate */
		filename[localHeader.fileNameLength] = '\0';
	} else { /* skip filename */
		if (zip->seek(zip, localHeader.fileNameLength, SEEK_CUR))
			return Z_ERRNO;
	}

	if (localHeader.extraFieldLength) {
		if (zip->seek(zip, localHeader.extraFieldLength, SEEK_CUR))
			return Z_ERRNO;
	}

	/*
	 * For now, silently ignore bit flags and hope ZLIB can uncompress
	 *  if (localHeader.generalPurposeBitFlag)
	 *     return Z_ERRNO; // Flags not supported
	 */

	if (localHeader.compressionMethod == 0 &&
		(localHeader.compressedSize != localHeader.uncompressedSize))
		/* Method is "store" but sizes indicate otherwise, abort */
		return Z_ERRNO;

	memcpy(header, &localHeader.compressionMethod, sizeof(JZFileHeader));
	header->offset = 0; /* not used in local context */

	return Z_OK;
}

/**
 * jzReadData:
 * Read data from file stream, described by header, to preallocated buffer
 */
int jzReadData(JZFile *zip, JZFileHeader *header, void *buffer)
{
	unsigned char *bytes = (unsigned char *)buffer;
	long compressedLeft, uncompressedLeft;
	z_stream strm;
	int ret;

	if (header->compressionMethod == 0) { /* Store - just read it */
		if (zip->read(zip, buffer, header->uncompressedSize) <
			header->uncompressedSize || zip->error(zip))
		return Z_ERRNO;
	} else if (header->compressionMethod == 8) { /* Deflate - using zlib */
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;

		strm.avail_in = 0;
		strm.next_in = Z_NULL;

		/*
		 * Use inflateInit2 with negative window bits
		 * to indicate raw data
		 */
		ret = inflateInit2(&strm, -MAX_WBITS);
		if (ret != Z_OK)
			return ret; /* Zlib errors are negative */

		/* Inflate compressed data */
		for (compressedLeft = header->compressedSize,
		uncompressedLeft = header->uncompressedSize;
		compressedLeft && uncompressedLeft && ret != Z_STREAM_END;
		compressedLeft -= strm.avail_in) {
			/* Read next chunk */
			strm.avail_in = zip->read(zip, jzBuffer,
				(sizeof(jzBuffer) < compressedLeft) ?
				sizeof(jzBuffer) : compressedLeft);

			if (strm.avail_in == 0 || zip->error(zip)) {
				inflateEnd(&strm);
				return Z_ERRNO;
			}

			strm.next_in = jzBuffer;
			strm.avail_out = uncompressedLeft;
			strm.next_out = bytes;

			/* inflate will change avail_in */
			compressedLeft -= strm.avail_in;

			ret = inflate(&strm, Z_NO_FLUSH);

			/* shouldn't happen */
			if (ret == Z_STREAM_ERROR)
				return ret;

			switch (ret) {
			case Z_NEED_DICT:
				/* and fall through */
				ret = Z_DATA_ERROR;
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return ret;
			}

			/* bytes uncompressed */
			bytes += uncompressedLeft - strm.avail_out;
			uncompressedLeft = strm.avail_out;
		}

		inflateEnd(&strm);
	} else {
		return Z_ERRNO;
	}

	return Z_OK;
}

/**
 * StdioJZFile:
 */
typedef struct {
	JZFile handle;
	FILE *fp;
} StdioJZFile;

/**
 * stdio_read_file_handle_read:
 */
static size_t stdio_read_file_handle_read(JZFile *file, void *buf, size_t size)
{
	StdioJZFile *handle = (StdioJZFile *)file;

	return fread(buf, 1, size, handle->fp);
}

/**
 * stdio_read_file_handle_tell:
 */
static size_t stdio_read_file_handle_tell(JZFile *file)
{
	StdioJZFile *handle = (StdioJZFile *)file;

	return ftell(handle->fp);
}

/**
 * stdio_read_file_handle_seek:
 */
static int stdio_read_file_handle_seek(JZFile *file, size_t offset, int whence)
{
	StdioJZFile *handle = (StdioJZFile *)file;

	return fseek(handle->fp, offset, whence);
}

/**
 * stdio_read_file_handle_error:
 */
static int stdio_read_file_handle_error(JZFile *file)
{
	StdioJZFile *handle = (StdioJZFile *)file;

	return ferror(handle->fp);
}

/**
 * stdio_read_file_handle_close:
 */
static void stdio_read_file_handle_close(JZFile *file)
{
	StdioJZFile *handle = (StdioJZFile *)file;

	fclose(handle->fp);
	free(file);
}

/**
 * jzfile_from_stdio_file:
 */
JZFile *jzfile_from_stdio_file(FILE *fp)
{
	StdioJZFile *handle = (StdioJZFile *)malloc(sizeof(StdioJZFile));

	handle->handle.read = stdio_read_file_handle_read;
	handle->handle.tell = stdio_read_file_handle_tell;
	handle->handle.seek = stdio_read_file_handle_seek;
	handle->handle.error = stdio_read_file_handle_error;
	handle->handle.close = stdio_read_file_handle_close;
	handle->fp = fp;

	return &(handle->handle);
}

#define BUFF_SIZE   1024

/**
 * util_make_path:
 */
static int util_make_path(const char *path, mode_t mode)
{
	struct stat st;
	char dir[BUFF_SIZE];
	char *s, *e = NULL;
	char c[2], te = 0;
	int err;

	err = stat(path, &st);
	if (!err && errno == EEXIST) {
		if (!S_ISDIR(st.st_mode)) {
			fprintf(stderr,
				"Exist %s but not directory to decompress\n",
				path);
			return -EINVAL;
		}
		return 0;
	}

	/* to modify const char */
	strcpy(dir, path);
	s = e = dir;

	if (strcmp(s, ".") == 0 || strcmp(s, "/") == 0)
		return 0;

	c[0] = dir[0];
	c[1] = dir[1];

	/* reverse search separator back */
	e = strrchr(s, '/');

	/* terminate string at this point */
	if (!e || s == e) {
		dir[0] = '.';
		dir[1] = '\0';
	} else {
		te = dir[e - s];
		dir[e - s] = '\0';
	}

	err = util_make_path(dir, mode);
	if (err == -1 && errno != EEXIST)
		goto out;

	/* restore string */
	dir[0] = c[0];
	dir[1] = c[1];
	if (te)
		dir[e - s] = te;

	/* make directory */
	err = mkdir(dir, mode);
	if (err == -1 && errno != EEXIST)
		return err;

out:
	return 0;
}

/**
 * zip_write_file:
 */
static int zip_write_file(zip_t *zip, char *filename, void *data, long bytes)
{
	FILE *out;
	char path[BUFF_SIZE];
	const char *dir = zip->dir;
	char *s, *e;

	/* decomprss path */
	if (dir) {
		sprintf(path, "%s/%s", dir, filename);
		filename = path;
	}

	s = filename;

	/* empty filename or directory entry */
	if (!strlen(filename))
		return -EINVAL;

	e = strrchr(s, '/');
	if (e) {
		if (s != e) {
			/* terminate string at this point */
			path[e-s] = '\0';
			util_make_path(path, 0755);
			/* Put the separator back */
			filename[e-s] = '/';
		}
	}

	out = fopen(filename, "wb");
	if (!out) {
		fprintf(stderr,
			"Open %s for writing zip!\n",
			filename);
		return errno;
	}

	fwrite(data, 1, bytes, out);
	fclose(out);

	return 0;
}

/**
 * zip_process_file:
 */
static int zip_process_file(zip_t *zip)
{
	JZFileHeader header;
	char filename[1024];
	unsigned char *data;
	JZFile *jzfp = zip->jzfp;
	int ret = 0;

	if (jzReadLocalFileHeader(jzfp, &header, filename, sizeof(filename))) {
		fprintf(stderr, "Failed read zip local file header!\n");
		return -1;
	}

	data = (unsigned char *)malloc(header.uncompressedSize);
	if (data == NULL) {
		fprintf(stderr, "Failed zip data allocate memory!\n");
		return -1;
	}

#ifdef DEBUG
	fprintf(stdout, "ZIP: %s, %d / %d bytes at offset %08X [%p]\n",
		filename, header.compressedSize, header.uncompressedSize,
		header.offset, data);
#endif

	if (jzReadData(jzfp, &header, data) != Z_OK) {
		fprintf(stderr, "Failed zip read file data!\n");
		free(data);
		return -1;
	}

	if (zip->dir)
		ret = zip_write_file(zip, filename,
				data, header.uncompressedSize);

	zip->decomps[zip->entries] = (unsigned char *)data;
	zip->entries++;

	return ret;
}

/**
 * zip_callback:
 */
static int zip_callback(JZFile *jzfp, int idx, JZFileHeader *header,
			char *filename, void *user_data)
{
	long offset;
	zip_t *zip = user_data;

	/* store current position */
	offset = jzfp->tell(jzfp);

	if (jzfp->seek(jzfp, header->offset, SEEK_SET)) {
		fprintf(stderr, "Cannot seek in zip file!\n");
		return 0; /* abort */
	}

	/* alters file offset */
	zip_process_file(zip);

	/* return to position */
	jzfp->seek(jzfp, offset, SEEK_SET);

	/* continue */
	return 1;
}

/**
 * zip_decompress:
 */
zip_t *jzip_decompress(const char *todir, const char *filename)
{
	FILE *fp;
	zip_t *zip = NULL;
	JZEndRecord record;
	int ret;

	fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "Failed fopen %s\n", filename);
		return NULL;
	}

	zip = (zip_t *)malloc(sizeof(zip_t));
	if (!zip) {
		fprintf(stderr, "Failed zip malloc %d\n", (int)sizeof(zip_t));
		goto cleanup_fp;
	}
	memset(zip, 0, sizeof(zip_t));

	zip->dir = todir;
	zip->entries = 0;
	zip->decomps = NULL;

	zip->jzfp = jzfile_from_stdio_file(fp);
	if (!zip->jzfp) {
		fprintf(stderr, "not exist %s\n", filename);
		goto cleanup_fp;
	}

	if (jzReadEndRecord(zip->jzfp, &record)) {
		fprintf(stderr, "Failed read ZIP file end record.\n");
		goto cleanup_zip;
	}

	zip->decomps = malloc(sizeof(unsigned char *) * record.numEntries);
	if (!zip->decomps) {
		fprintf(stderr, "Failed zip entries malloc %d",
			record.numEntries);
		goto cleanup_zip;
	}

	ret = jzReadCentralDirectory(zip->jzfp, &record, zip_callback, zip);
	if (ret) {
		fprintf(stderr,
			"Failed read ZIP file central record.\n");
		goto cleanup_zip;
	}

	fclose(fp);

	return zip;

cleanup_zip:
	zip->jzfp->close(zip->jzfp);

cleanup_fp:
	fclose(fp);

	free(zip->decomps);
	free(zip);

	return NULL;
}

/**
 * zip_release:
 */
void jzip_release(zip_t *zip)
{
	int entries;

	if (!zip)
		return;

	entries = zip->entries;

	while (entries-- > 0)
		free(zip->decomps[entries]);

	free(zip->decomps);
	free(zip);
}

