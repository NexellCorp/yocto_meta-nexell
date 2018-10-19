#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <unistd.h>
#include    "def.h"
#include	"erase.h"
#include	"spieeprom.h"

extern int max_part;
extern int max_tbl_part;
extern struct update_part f_part[UPDATE_DEV_PART_MAX];
extern int check_mmc_f_part(struct update_part *f_part);
extern int is_mounted_mmc (void);
extern unsigned long long int get_mmc_blk_size();
#ifdef  PSPLASH_UI
extern int print_msg(char * msg);
extern int print_progress(unsigned int msg);
#endif

static int erase_sd(int offset, unsigned long long img_size)
{
    int fd = open(MMC_DEV_NAME, O_RDWR);
    int ret = 0;
	char mem[BLK_SIZE];
	unsigned long long erase_size=img_size;
	int cnt=0;

    if (fd < 0) {
        fprintf(stderr, "%s: failed to open %s\n", __func__, MMC_DEV_NAME);
        return -1;
    }

    ret = lseek(fd, offset, SEEK_SET);
    if (ret != offset) {
        fprintf(stderr, "%s: lseek returned invalid offset(%d/%d)\n", __func__, ret, offset);
        close(fd);
        return -1;
    }

	memset(mem, 0, sizeof(mem));

	do {
	    ret = write(fd, mem, sizeof(mem));
	    if (ret != sizeof(mem)) {
	        fprintf(stderr, "%s: write returned invalid size(%d/%d)\n", __func__, ret, img_size);
	        close(fd);
    	    return -1;
	    }
		erase_size -= sizeof(mem);
	}while(erase_size > 0);

    close(fd);

    printf("Succeed to erase of %s, offset %d, size %lld\n", MMC_DEV_NAME, offset, img_size);
    return 0;

}


static int erase_eeprom(struct update_part * f_part) {

	int fd;
	struct update_part *fp = f_part;
    #ifdef PSPLASH_UI
    char psplash_msg[128];
    #endif

	#ifdef PSPLASH_UI
	memset(psplash_msg, 0 , sizeof(psplash_msg));
	#endif

    fd = open(SPIROM_DEVICE, O_RDWR);
    if (fd < 0) {
        printf("%s: can't open %s\n", __func__, SPIROM_DEVICE);
        return -EINVAL;
    }

    #ifdef  PSPLASH_UI
	sprintf(psplash_msg,"Erase %s \n",fp->partition_name);
    print_msg(psplash_msg);
    //print_progress(20);
    #endif

	eeprom_init(fd);

	if( !strncmp(fp->partition_name,"2ndboot",7) ) {
        eeprom_write_enable(fd);
        eeprom_blk_erase(fd, BLOCK_32K_ERASE, 0);   //Block Erase 64KB
	}
	else if( !strncmp(fp->partition_name,"bootloader",10) ) {
		int offset = fp->start;
		int size = fp->length;

        do {
            eeprom_write_enable(fd);
            eeprom_blk_erase(fd ,BLOCK_32K_ERASE ,offset);  //Block Erase 32KB
            offset += ERASE_BLOCK_SIZE;
            size -= ERASE_BLOCK_SIZE;
            if(size < 0)
                size = 0;
        } while (size >0);
	}
	return 1;

}

static int erase_mmc(struct update_part * f_part) {

	char erase_cmd[128];
	char mmc_node[128];
	struct update_part *fp = f_part;
	#ifdef PSPLASH_UI
	char psplash_msg[128];
	#endif

	memset(erase_cmd, 0 , sizeof(erase_cmd));
	memset(mmc_node, 0 , sizeof(mmc_node));
	#ifdef PSPLASH_UI
	memset(psplash_msg, 0 , sizeof(psplash_msg));
	#endif

    if(fp->part_num == max_part && fp->length == 0) {
        unsigned long long total_blk;
        total_blk = get_mmc_blk_size();
        fp->length = (total_blk * 2 * BLK_SIZE ) - (fp->start);
    }

	printf("%s : 0x%llx : 0x%llx \n", fp->partition_name, fp->start, fp->length);


	#ifdef  PSPLASH_UI
	sprintf(psplash_msg,"Erase %s \n",fp->partition_name);
    print_msg(psplash_msg);
    #endif

	if(fp->part_num == 0) {
		//raw format via ddpt
		//sprintf(erase_cmd, "%s if=/dev/zero of=%s bs=%d seek=%lld count=%lld",
		//	DDPT, MMC_DEV_NAME, BLK_SIZE, fp->start/BLK_SIZE, fp->length/BLK_SIZE);
		erase_sd(fp->start, fp->length);
	}
	else {
		//use mkfs utils
		if(fp->fs_type & UPDATE_FS_MASK) {
			switch(fp->fs_type) {
				case UPDATE_FS_EXT4 :
					sprintf(mmc_node, "%sp%d", MMC_DEV_NAME, fp->part_num);
					sprintf(erase_cmd, "%s -F %s", MKFS_EXT4, mmc_node);
					printf("erase_cmd : %s \n", erase_cmd);

				 if (system(erase_cmd) <  0) {
					printf("sh: %s error \n", erase_cmd);
				    }
					break;
				case UPDATE_FS_RAW_PART :
					//sprintf(mmc_node, "%sp%d", MMC_DEV_NAME, fp->part_num);
					//sprintf(erase_cmd, "%s if=/dev/zero of=%s bs=%d count=%lld",
					//	DDPT, mmc_node, BLK_SIZE, fp->length/BLK_SIZE);
					erase_sd(fp->start, fp->length);
					break;
			}
		}
	}



    #ifdef  PSPLASH_UI
    //print_progress(100);
	#endif


	return 1;
}

int do_erase(char * cmdline) {

	char cmd[20];
	char arg[20];
	int i;
	int flags=0;
	struct update_part *fp = f_part;

	memset(cmd, 0 , sizeof(cmd));
	memset(arg, 0 , sizeof(arg));

	sscanf(cmdline, "%s %s", cmd, arg);

	printf("cmd = %s \n", cmd);
	printf("arg = %s \n", arg);

	if( strncmp(cmd, "erase", 5)) {
		printf("cmd is not erase \n");
		return -1;
	}

	if( !strncmp(arg,"all",3)) {
		/* TODO erase all partition */
		printf("erase all \n");
		flags = 1;
	}

    if ( check_mmc_f_part(fp) ) {
    	// check mmcblk0 mounted and umount
	    if(is_mounted_mmc()) {
    	    //umount mmcblk0*
	        if (system("/bin/umount /dev/mmcblk0*") <  0) {
        	        printf("sh: umount error \n");
    	    }

	    }
	}


	for(i=0; i<max_tbl_part; i++, fp++)
    {
		if(!flags) {
			if(fp->partition_name != NULL &&
				!strcmp(arg, fp->partition_name)) {

				printf("\033[41m erase %d : %s : %s \033[0m\n", i, arg, fp->partition_name);

				if(!strncmp(fp->device, "eeprom" , 6) )	{
					/* TODO eeprom erase routine */
					 if (erase_eeprom(fp) > 0 ) {
                        printf("erase %s finished \n", arg);
                    }

				}
				else if(!strncmp(fp->device, "mmc" , 3) )	{
					if (erase_mmc(fp) > 0 ) {
						printf("erase %s finished \n", arg);
					}
				}
				else {
					printf("device is not eeprom or mmc \n");
					return -1;
				}

			}
		}
		else {
               printf("\033[41m erase %d : %s : %s \033[0m\n", i, arg, fp->partition_name);

                if(!strncmp(fp->device, "eeprom" , 6) ) {
                    /* TODO eeprom erase routine */
                     if (erase_eeprom(fp) > 0 ) {
                        printf("erase %s finished \n", arg);
                    }
                }
                else if(!strncmp(fp->device, "mmc" , 3) )   {
                    if (erase_mmc(fp) > 0 ) {
                        printf("erase %s finished \n", arg);
                    }
                }
                else {
                    printf("device is not eeprom or mmc \n");
                    return -1;
                }
		}

	}
	return 0;
}

