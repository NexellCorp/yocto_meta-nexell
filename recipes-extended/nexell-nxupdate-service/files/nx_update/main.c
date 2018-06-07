#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<string.h>
#include 	<sys/types.h>
#include 	<sys/stat.h>
#include	<sys/reboot.h>
#include	<sys/syscall.h>
#include	<linux/reboot.h>
#include 	<unistd.h>
#include	<time.h>
#include	"def.h"
#include	"proceed_update_cmd.h"

#ifdef  PSPLASH_UI
extern int psplash_init();
extern int psplash_close();
extern int print_msg(char * msg);
extern int print_progress(unsigned int msg);
#endif

/* support fs type */
#define  FS_TYPE_NUM	9
static struct update_fs_type f_part_fs[FS_TYPE_NUM] = {
    { "2nd"     , UPDATE_FS_2NDBOOT  },
    { "boot"    , UPDATE_FS_BOOT     },
    { "env"     , UPDATE_FS_ENV      },
    { "raw"     , UPDATE_FS_RAW      },
    { "fat"     , UPDATE_FS_FAT      },
    { "ext4"    , UPDATE_FS_EXT4     },
    { "emmc"    , UPDATE_FS_RAW_PART },
    { "ubi"     , UPDATE_FS_UBI      },
    { "ubifs"   , UPDATE_FS_UBIFS    },
};



struct update_part f_part[UPDATE_DEV_PART_MAX];
int max_part;
int max_tbl_part;

static inline void update_parse_comment(const char *str, const char **ret) {

    const char *p = str, *r;
    do {
        if (!(r = strchr(p, '#')))
            break;
        r++;
        if (!(p = strchr(r, '\n'))) {
            printf("---- not end comments '#' ----\n");
            break;
        }
        p++;
    } while (1);
    /* for next */
    *ret = p;
}
static inline int update_parse_string(const char *s, const char *e, char *b, int len) {

    int l, a = 0;
    do { while (0x20 == *s || 0x09 == *s || 0x0a == *s) { s++; } } while(0);

    if (0x20 == *(e-1) || 0x09 == *(e-1))
        do { e--; while (0x20 == *e || 0x09 == *e) { e--; }; a = 1; } while(0);

    l = (e - s + a);
    if (l > len) {
        printf("-- Not enough buffer %d for string len %d [%s] --\n", len, l, s);
        return -1;
    }

    strncpy(b, s, l);
    b[l] = 0;

    return l;
}

static inline void update_sort_string(char *p, int len) {
    int i, j;
    for (i = 0, j = 0; len > i; i++) {
        if (0x20 != p[i] && 0x09 != p[i] && 0x0A != p[i])
            p[j++] = p[i];
    }
    p[j] = 0;
}


static int update_parse_part_head(const char *parts, const char **ret) {
    const char *p = parts;
    int len = strlen("flash=");

    if (!(p = strstr(p, "flash=")))
        return -1;

    *ret = p + len;
    return 0;
}
static const char *update_sdcard_get_string(const char *ptable_str, int search_c, char * buf, int buf_size)
{
    const char *id, *c;

    id = ptable_str;
    c = strchr(id, search_c);

    memset(buf, 0x0, buf_size);
    update_parse_string(id, c, buf, buf_size);

    return c+1;
}

static const char *update_get_string(const char *ptable_str, int search_c, char * buf, int buf_size)
{
    const char *id, *c;

    id = ptable_str;
    c = strchr(id, search_c);

    memset(buf, 0x0, buf_size);
    update_parse_string(id, c, buf, buf_size);

    return c+1;
}



static int part_lists_make(const char *ptable_str, int ptable_str_len)
{
	struct update_part *fp = f_part;
    const char *p = ptable_str;
    int len = ptable_str_len;
	char str[32];
    int err = -1;
	int i=0, j=0;
	int part_num = 0;

    printf("\n---part_lists_make ---\n");
    update_parse_comment(p, &p);
    update_sort_string((char*)p, len);

    for(i=0; i<UPDATE_DEV_PART_MAX; i++, fp++)
    {
        struct update_fs_type *fs = f_part_fs;

        if (update_parse_part_head(p, &p)) {
            break;
        }

        p = update_get_string(p, ',', str, sizeof(str));
        strcpy(fp->device, str);

        p = update_get_string(p, ':', str, sizeof(str));
        fp->dev_no = strtoul(str, NULL, 10);

        p = update_get_string(p, ':', str, sizeof(str));
        strcpy(fp->partition_name, str);


        p = update_get_string(p, ':', str, sizeof(str));

        for (j=0; j < FS_TYPE_NUM ; j++, fs++) {

            if (strcmp(fs->name, str) == 0) {
                fp->fs_type = fs->fs_type;

                if(fp->fs_type & UPDATE_FS_MASK)
                {
                    part_num++;
					//part num 4 is extended partition
					if(part_num == 4) {
						part_num++;
					}

                    fp->part_num = part_num;
					max_part = part_num;
                }
				max_tbl_part++;
                break;
            }
        }

        p = update_sdcard_get_string(p, ',', str, sizeof(str));
        fp->start = strtoul(str, NULL, 16);

        p = update_sdcard_get_string(p, ':', str, sizeof(str));
        fp->length = strtoul(str, NULL, 16);

        p = update_sdcard_get_string(p, ';', str, sizeof(str));
        strcpy(fp->file_name, str);
        err = 0;
    }
    return err;
}

static void update_part_lists_print(void) {

    struct update_part *fp = f_part;
    int i;

    printf("\nPartitions:\n");

    for(i=0; i<UPDATE_DEV_PART_MAX; i++, fp++)
    {
        if(!strcmp(fp->device, "")) break;

        printf("  %s.%d : %s : %s : 0x%llx, 0x%llx : %s , %d\n",
                    fp->device, fp->dev_no,
                    fp->partition_name, UPDATE_FS_MASK&fp->fs_type?"fs":"img",
                    fp->start, fp->length,
                    fp->file_name, fp->part_num);
    }
}




void usage(void) {
	printf("./nx_update [path to partmap_update.txt] \n");
	printf("ex) ./nx_update /mnt/1");
	printf("\n");
	return ;
}

int main(int argc, char *argv[]) {

	FILE *partmap;
	char file_path[100];
	char *p;
	size_t r_size=0;

	if(argc < 2) {
		printf("bad argument \n");
		usage();
		return -1;
	}

	sprintf(file_path,"%s/%s",argv[1],PARTMAP_UPDATE);

	partmap = fopen (file_path, "r");

	if(partmap) {
		struct stat buf;
		stat(file_path, &buf);
		p = (char *) malloc (buf.st_size * sizeof(char));
		r_size = fread(p, 1, buf.st_size, partmap);
		if(r_size > 0) {
			#ifdef  PSPLASH_UI
			psplash_init();
			#endif
			part_lists_make(p,buf.st_size);
			update_part_lists_print();
			if (proceed_update_cmd_sh(argv[1], f_part) < 0)
				goto exit1;
		}
	}
	else {
		printf("File %s open Failed \n", file_path );
		print_msg("Update failed : Error code = 1000");
		goto exit2;
	}

	sync();

	#ifdef  PSPLASH_UI
    print_progress(100);
	print_msg("Updates finished");
	sleep(1);
	print_msg("Reboot after 3 seconds");
	sleep(1);
	print_msg("Reboot after 2 seconds");
	sleep(1);
	print_msg("Reboot after 1 seconds");
	sleep(1);
	print_msg("Reboot after 0 seconds");
	sleep(1);
	system("reboot");
    #endif

exit1:
	fclose(partmap);
	free(p);
exit2:
	psplash_close();

	return 0;
}

