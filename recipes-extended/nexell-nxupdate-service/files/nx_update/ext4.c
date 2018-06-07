#include 	<stdio.h>
#include 	<errno.h>
#include 	<stdarg.h>
#include 	<stdlib.h>
#include 	<string.h>
#include 	<sys/types.h>
#include 	<sys/stat.h>
#include 	<fcntl.h>
#include 	<unistd.h>
#include    "def.h"


#ifdef DEBUG_EXT4
#define ext4_printf(args...)    printf(args)
#else
#define ext4_printf(args...)    do { } while(0)
#endif

#ifdef  PSPLASH_UI
extern int print_msg(char * msg);
extern int print_progress(unsigned int msg);
#endif



int check_ext4_compress(char *source, unsigned long long part_size)
{
	ext4_file_header *file_header;

    file_header = (ext4_file_header*)source;

    if (file_header->magic != EXT4_FILE_HEADER_MAGIC) {
		printf("header magic = %02x\n", file_header->magic);
        return -1;
    }

    if (file_header->major != EXT4_FILE_HEADER_MAJOR) {
        printf("Invalid Version Info! 0x%2x\n", file_header->major);
        return -1;
    }

    if (file_header->file_header_size != EXT4_FILE_HEADER_SIZE) {
        printf("Invalid File Header Size! 0x%8x\n",
                                file_header->file_header_size);
        return -1;
    }

    if (file_header->chunk_header_size != EXT4_CHUNK_HEADER_SIZE) {
        printf("Invalid Chunk Header Size! 0x%8x\n",
                                file_header->chunk_header_size);
        return -1;
    }

    if (file_header->block_size != EXT4_FILE_BLOCK_SIZE) {
        printf("Invalid Block Size! 0x%8x\n", file_header->block_size);
        return -1;
    }

    if ((part_size/file_header->block_size)  < file_header->total_blocks) {
        printf("Invalid Volume Size! Image is bigger than partition size!\n");
        printf("partion size %lld , image size %lld \n",
            part_size, (unsigned long long )file_header->total_blocks * file_header->block_size);
        printf("Hang...\n");
        while(1);
    }

	/* image is compressed ext4 */
    return 0;

}


int writeCompressedExt4Image(char *imgBase, size_t imgSize, int fd) {
    off_t sectorSize;
    int totalChunks;
    ext4_chunk_header *chunkHeader;
    ext4_file_header *fileHeader;
    char *base = imgBase;
    size_t writeCount;

	#ifdef  PSPLASH_UI
	lldiv_t div_value;
	unsigned int progress=0;
	int cnt=0;
	#endif

    fileHeader = (ext4_file_header *)base;
    totalChunks = fileHeader->total_chunks;

    printf("%s: total chunk = %d\n", __func__, totalChunks);

    base += EXT4_FILE_HEADER_SIZE;

    while (totalChunks) {
         chunkHeader = (ext4_chunk_header *)base;
         sectorSize = (chunkHeader->chunk_size * fileHeader->block_size);
         ext4_printf("sectorSize: %d\n", sectorSize);
		#ifdef  PSPLASH_UI
		 div_value = lldiv(cnt*100, fileHeader->total_chunks);
		 progress = (unsigned int) div_value.quot;
		//printf("progress = %d \n",progress);
		 print_progress(progress);
		#endif

         usleep(100);

         switch (chunkHeader->type) {
         case EXT4_CHUNK_TYPE_RAW:
             ext4_printf("raw chunk\n");
             writeCount = write(fd, base + EXT4_CHUNK_HEADER_SIZE, sectorSize);
             if (writeCount != sectorSize) {
                  ext4_printf("%s: write error, mismatching count %d/%d\n", __func__, sectorSize, writeCount);
                  return -EIO;
             }
             break;

         case EXT4_CHUNK_TYPE_FILL:
             ext4_printf("fill chunk\n");
             lseek(fd, sectorSize, SEEK_CUR);
             break;

         case EXT4_CHUNK_TYPE_NONE:
             ext4_printf("none chunk\n");
             lseek(fd, sectorSize, SEEK_CUR);
             break;

         default:
             ext4_printf("unknown chunk\n");
             lseek(fd, sectorSize, SEEK_CUR);
             break;
         }
		#ifdef  PSPLASH_UI
		 cnt++;
		#endif
         totalChunks--;
         base += chunkHeader->total_size;
    }

    printf("%s: write done\n", __func__);
    return 0;
}



