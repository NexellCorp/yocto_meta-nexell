#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <unistd.h>
#include 	<errno.h>
#include 	<fcntl.h>
#include 	<math.h>
#include    "def.h"
#include	"flash.h"
#include	"spieeprom.h"

extern int max_part;
extern int max_tbl_part;
extern struct update_part f_part[UPDATE_DEV_PART_MAX];
extern int check_mmc_f_part(struct update_part *f_part);
extern int is_mounted_mmc (void);
extern unsigned long long int get_mmc_blk_size();
extern int check_ext4_compress(char *source, unsigned long long part_size);
extern int writeCompressedExt4Image(char *imgBase, size_t imgSize, int fd);
#ifdef  PSPLASH_UI
extern int print_msg(char * msg);
extern int print_progress(unsigned int msg);
#endif

static int update_sd_from_mem(int offset, char *mem, size_t img_size)
{
    int fd = open(MMC_DEV_NAME, O_RDWR);
    int ret = 0;
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

    ret = write(fd, mem, img_size);
    if (ret != img_size) {
        fprintf(stderr, "%s: write returned invalid size(%d/%d)\n", __func__, ret, img_size);
        close(fd);
        return -1;
    }

    close(fd);

    printf("Succeed to update of %s, offset %d, size %d\n", MMC_DEV_NAME, offset, img_size);
	return 0;

}

static int append_uboot_header(struct update_part * f_part, unsigned int fsize) {

	char flash_cmd[128];
	struct update_part *fp = f_part;
	//FILE * boot_header_file;

    struct boot_dev_head head;
    struct boot_dev_head *bh = &head;
    struct boot_dev_mmc *bd = (struct boot_dev_mmc *)&bh->bdi;
	int ret = 0;

	memset(flash_cmd, 0 , sizeof(flash_cmd));

    memset((void*)&head, 0x00, sizeof(head));

    bh->load_addr = (unsigned int) U_BOOT_LOAD_ADDRESS;
    bh->jump_addr = bh->load_addr;
    bh->load_size = (unsigned int) fsize;
    bh->signature = SIGNATURE_ID;
    bd->port_no   = fp->dev_no;

    printf("head boot dev  = %d \n", bd->port_no);
    printf("head load addr = 0x%08x \n", bh->load_addr);
    printf("head load size = 0x%08x \n", bh->load_size);
    printf("head signature = 0x%08x \n", bh->signature);

	if(!strncmp(fp->device,"mmc",3) ){

		update_sd_from_mem(fp->start, (char *)bh, sizeof(head));
		#if 0
	    boot_header_file = fopen(MMC_DEV_NAME,"w");
		if(boot_header_file) {

			fseek(boot_header_file, fp->start, SEEK_SET);

			fwrite(bh, 1, sizeof(head), boot_header_file);

		    fclose(boot_header_file);
			ret = 1;
		}
		else {
			printf("File %s open Failed \n", MMC_DEV_NAME );
	        ret = -1;
		}
		#endif
	}
	return ret;

}
static int mmc_open(char *path) {

	int fd =0;

    if (0 != access(path, F_OK)) {
        printf("cannot access file (%s).\n", path);
        return -errno;
    }

    fd = open(path, O_RDWR);
	return fd;
}
static int mmc_close(int fd) {

	close(fd);
	return 0;
}

unsigned int get_fcs(unsigned int fcs, unsigned char data)
{
    register int i;
    fcs ^= (unsigned int)data;

    for(i=0; i<8; i++)
    {
        if(fcs & 0x01) fcs ^= POLY; fcs >>= 1;
    }
    return fcs;
}

static int flash_eeprom(char *file_path, struct update_part * f_part) {
	int ret,i;
    int size = 0;
    uint8_t *buf, *wbuf;
    uint8_t *verify_out;
	struct stat	fstat;
    unsigned int *p;
    int CRC = 0;
    int w_size;
    int fd;
    int ffd;
	char path[100];
	struct update_part *fp = f_part;
	lldiv_t div_value;
	volatile unsigned int progress=0;
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

	eeprom_init(fd);

	sprintf(path, "%s/%s",file_path,fp->file_name);

	printf("file_name = %s \n",path);

    #ifdef  PSPLASH_UI
    sprintf(psplash_msg,"Writing %s \n",fp->file_name);
    print_msg(psplash_msg);
    #endif


    ffd = open(path, O_RDWR);
    if (ffd < 0) {
         printf("%s: can't open %s\n", __func__, path);
         return -EINVAL;
    }
	stat(path, &fstat);

	if( !strncmp(fp->partition_name,"2ndboot",7) ) {

        buf = (uint8_t *)malloc(fstat.st_size);
        p = (unsigned int *)buf;
        verify_out = (uint8_t *)malloc(fstat.st_size);

        ret = read(ffd,buf,fstat.st_size);
        if(ret < 0) {
            printf("READ fail secondboot %s \n",path);
			close(fd);
			free(verify_out);
			return -1;
        }
        for (i = 0; (SECBOOT_SIZE-16) > i; i++ )
            CRC = get_fcs(CRC, buf[i]);

        p[(SECBOOT_SIZE-16)/4] = CRC;

        eeprom_write_enable(fd);
		eeprom_read_rdsr(fd);
        eeprom_write_enable(fd);
        eeprom_blk_erase(fd, BLOCK_32K_ERASE, fp->start);   //Block Erase 64KB
        eeprom_write_enable(fd);

        size = SECBOOT_SIZE;

        eeprom_write(fd, fp->start, (char *) buf, fp->length);  //Write Data
		/* verify */
		#ifdef  PSPLASH_UI
    	sprintf(psplash_msg,"Verify %s \n",fp->partition_name);
	    print_msg(psplash_msg);
	    #endif

		for(i=0; i < (fstat.st_size) ; i+=2048 ) {
			#ifdef PSPLASH_UI
			div_value = lldiv(i*100, fstat.st_size);
            progress = (unsigned int) div_value.quot;
    		print_progress(progress);
			#endif
	        eeprom_read(fd, fp->start+i, (char *) verify_out+i, 2048);
		}

		ret = memcmp(buf, verify_out, fstat.st_size);
	    printf("\n\ncompare %s %d \n",ret ? "fail":"ok",ret);
		#ifdef  PSPLASH_UI
		if(!ret){
	    	sprintf(psplash_msg,"Verify %s OK\n",fp->partition_name);
		    print_msg(psplash_msg);
		}
	    #endif

    	if(fd)
        	close(fd);

    	if(buf)
        	free(buf);

		if(verify_out)
        	free(verify_out);

    } else if( !strncmp(fp->partition_name,"bootloader",10) ) {
        int offset=0;
        struct boot_dev_head head;
        struct boot_dev_head *bh = &head;
        struct boot_dev_eeprom *bd = (struct boot_dev_eeprom *)&bh->bdi;
        int len = sizeof(head);
        int load = TEXT_BASE;
        w_size = 0;

        buf = (uint8_t *)malloc(fp->length);
        ret = read(ffd, buf, fstat.st_size);
        verify_out = (uint8_t *)malloc(fp->length);

        if (ret < 0) {
            printf("READ fail uboot %s \n",path);
			close(fd);
			free(buf);
			return -1;
        }
        size = ret;
        CRC =0;
        for (i = 0; size > i; i++ )
            CRC = get_fcs(CRC, buf[i]);
        memset(bh, 0xff, len);

        bh->load_addr = load;
        bh->jump_addr = bh->load_addr;
        bh->load_size = (int)size;
        bh->signature = SIGNATURE_ID;
        bd->addr_step = CONFIG_EEPROM_ADDRESS_STEP;
        bd->crc32 = CRC;

        wbuf = (uint8_t *)malloc(size+len);
        memcpy(wbuf,bh,len);
        memcpy(wbuf+len,buf,size);

        offset = fp->start  + w_size;
        size = size + len;
        do {
            #ifdef PSPLASH_UI
            div_value = lldiv(w_size*100, fstat.st_size);
            progress = (unsigned int) div_value.quot;
            print_progress(progress);
            #endif

            eeprom_write_enable(fd);
            eeprom_blk_erase(fd ,BLOCK_32K_ERASE ,offset);  //Block Erase 32KB

            eeprom_write_enable(fd);
            eeprom_write(fd,offset,(char *) wbuf+w_size,ERASE_BLOCK_SIZE);   //Write Data
            w_size += ERASE_BLOCK_SIZE;
            offset += ERASE_BLOCK_SIZE;
            size -= ERASE_BLOCK_SIZE;
            if(size < 0)
                size = 0;
        } while (size >0);


		#ifdef  PSPLASH_UI
    	sprintf(psplash_msg,"Verify %s \n",fp->partition_name);
	    print_msg(psplash_msg);
	    #endif

		for(i=0; i < (fp->length) ; i+=2048 ) {
			#ifdef PSPLASH_UI
			div_value = lldiv(i*100, fstat.st_size);
    	    progress = (unsigned int) div_value.quot;
   			print_progress(progress);
			#endif
            eeprom_read(fd, fp->start+sizeof(struct boot_dev_head)+i, (char *) verify_out+i, 2048);
        }

		ret = memcmp(buf, verify_out, fstat.st_size);
	    printf("\n\ncompare %s %d \n",ret ? "fail":"ok",ret);
		#ifdef  PSPLASH_UI
		if(!ret){
	    	sprintf(psplash_msg,"Verify %s OK\n",fp->partition_name);
		    print_msg(psplash_msg);
		}
	    #endif

		if(fd)
        	close(fd);

    	if(buf)
        	free(buf);

		if(wbuf)
        	free(wbuf);
	}

    return 1;

}
extern void DumpHex(const void* data, size_t size);
static int flash_mmc(char *file_path, struct update_part * f_part) {

	char flash_cmd[128];
	char mmc_node[128];
	char img_path[128];
	FILE	*img_file;
	char *buffer;
	char *verifybuf;
	#ifdef PSPLASH_UI
    char psplash_msg[128];
	lldiv_t	div_value;
	volatile unsigned int progress=0;
	volatile unsigned int progress2=0;
    #endif
	int ret = 0;
	struct stat fstat;
	unsigned long long length=0;
	unsigned long long n=0;
	int fd;

	struct update_part *fp = f_part;

	memset(flash_cmd, 0 , sizeof(flash_cmd));
	memset(mmc_node, 0 , sizeof(mmc_node));
	memset(img_path, 0 , sizeof(img_path));
	#ifdef PSPLASH_UI
    memset(psplash_msg, 0 , sizeof(psplash_msg));
    #endif


	if( (fp->part_num == max_part) && (fp->length == 0)) {
		unsigned long long total_blk=0;
		total_blk = get_mmc_blk_size();
		fp->length = (total_blk * 2 * BLK_SIZE ) - (fp->start);
	}

	printf("%s : 0x%llx : 0x%llx \n", fp->partition_name, fp->start, fp->length);
	#ifdef  PSPLASH_UI
	sprintf(psplash_msg,"Writing %s \n",fp->file_name);
    print_msg(psplash_msg);
    #endif


	if(fp->part_num == 0) {
		//raw format via ddpt

		//1. Read image file

		sprintf(img_path, "%s/%s",file_path, fp->file_name);
        img_file = fopen(img_path,"r");
        stat(img_path, &fstat);
        buffer = (char *) malloc (fstat.st_size * sizeof(char));
		memset(buffer, 0 , (fstat.st_size * sizeof(char)));
		fread(buffer, 1, (fstat.st_size *  sizeof(char) ), img_file);

		//DumpHex(buffer, fstat.st_size);
		//fclose(img_file);



//		if( !strncmp(fp->partition_name,"bootloader",10) )	{
			//TODO add u-boot header
//			if( append_uboot_header (fp, fstat.st_size) ) {
//				printf("write the u-boot header \n");
//			}

			//sprintf(flash_cmd, "%s if=%s/%s of=%s bs=%d seek=%lld",
			//	DDPT, file_path, fp->file_name, MMC_DEV_NAME, BLK_SIZE,
			//	(fp->start/BLK_SIZE) + 1 ); //1 block for u-boot header


//			update_sd_from_mem(fp->start+BLK_SIZE, buffer, fstat.st_size);

//		}
//		else {

			//sprintf(flash_cmd, "%s if=%s/%s of=%s bs=%d seek=%lld",
			//	DDPT, file_path, fp->file_name, MMC_DEV_NAME, BLK_SIZE,
			//	fp->start/BLK_SIZE);

			update_sd_from_mem(fp->start, buffer, fstat.st_size);
//		}
		//update_sd_from_mem(fp->start, buffer, fstat.st_size);

		printf("flash_cmd : %s \n", flash_cmd);

		sprintf(psplash_msg,"Writing %s offset : 0x%llx size = %d byte \n",fp->file_name, fp->start, (int) fstat.st_size);
		print_msg(psplash_msg);


		//if (system(flash_cmd) <  0) {
        //	printf("sh: %s error \n", flash_cmd);
        //}

		/* verify */
		#ifdef  PSPLASH_UI
    	sprintf(psplash_msg,"Verify %s \n",fp->partition_name);
	    print_msg(psplash_msg);
	    #endif

		sprintf(mmc_node, "%s", MMC_DEV_NAME);
		//printf("mmc_node = %s \n", mmc_node);
		fd = mmc_open(mmc_node);

		//sprintf(img_path, "%s/%s",file_path, fp->file_name);
		//img_file = fopen(img_path,"r");

		//stat(img_path, &fstat);

		//buffer = (char *) malloc (fstat.st_size * sizeof(char));
		verifybuf = (char *) malloc (fstat.st_size * sizeof(char));
		//memset(buffer, 0 , (fstat.st_size * sizeof(char)));
		memset(verifybuf, 0 , (fstat.st_size * sizeof(char)));



//		if( !strncmp(fp->partition_name,"bootloader",10) ) {
//			lseek(fd, fp->start + BLK_SIZE, SEEK_SET);
//		}
//		else {
			lseek(fd, fp->start, SEEK_SET);
//		}

		read(fd, verifybuf, fstat.st_size);


		//fread(buffer, 1, fstat.st_size, img_file);

		//DumpHex(verifybuf, fstat.st_size);

//		if( !strncmp(fp->partition_name,"bootloader",10) ) {
//			ret = memcmp(buffer, verifybuf, fstat.st_size - BLK_SIZE);
//		}
//		else {
			ret = memcmp(buffer, verifybuf, fstat.st_size);
//		}

	    printf("\n\n \033[41m compare %s %d \033[0m\n",ret ? "fail":"ok",ret);

		#ifdef  PSPLASH_UI
		if(!ret){
	    	sprintf(psplash_msg,"Verify %s OK\n",fp->partition_name);
		    print_msg(psplash_msg);
		}
	    #endif
		free(buffer);
		free(verifybuf);
		fclose(img_file);
		mmc_close(fd);

	}
	else {
		//filesystem write
		if(fp->fs_type & UPDATE_FS_MASK) {

            switch(fp->fs_type) {

                case UPDATE_FS_EXT4 : {
					sprintf(img_path, "%s/%s",file_path, fp->file_name);
			        stat(img_path, &fstat);
					printf("img_path = %s , %ld\n", img_path,fstat.st_size);
					buffer = (char *) malloc (fstat.st_size * sizeof(char));
					memset(buffer, 0 , (fstat.st_size * sizeof(char)));


					img_file = fopen(img_path,"r");
					length = fstat.st_size;
					#ifdef  PSPLASH_UI
					sprintf(psplash_msg,"Reading %s \n",fp->file_name);
				    print_msg(psplash_msg);
				   	print_progress(0);
				    #endif

					do {
						ret = fread(buffer+n, 1, RW_SIZE, img_file);
						length -= ret;
						n += ret;
						#ifdef  PSPLASH_UI
						div_value = lldiv(n*100, fstat.st_size);
						progress = (unsigned int) div_value.quot;
						if(progress !=0 && ( (int) progress > (int)(progress2) )) {
							//printf("read %d %lld %ld %d %d, \n", ret, n, fstat.st_size, progress, progress2 );
				    		print_progress(progress);
						}
						progress2 = progress;
						#endif
					}while(length);

					// TODO uncompress ext4
					sleep(1);
					#ifdef  PSPLASH_UI
                    sprintf(psplash_msg,"Uncompressed %s \n",fp->file_name);
                    print_msg(psplash_msg);
                    print_progress(0);
                    #endif

					ret = check_ext4_compress(buffer, fp->length);
				    if (0 == ret) {
						sprintf(mmc_node, "%sp%d", MMC_DEV_NAME, fp->part_num);
						printf("mmc_node = %s \n", mmc_node);
						fd = mmc_open(mmc_node);
						writeCompressedExt4Image(buffer, fstat.st_size, fd);
						mmc_close(fd);
					} else {
						free(buffer);
						fclose(img_file);
						return -1;
					}
					free(buffer);
					fclose(img_file);
					break;
				}
				default :
					break;
            }
		}
	}


	return 1;
}

int do_flash(char *file_path, char * cmdline) {

	char cmd[20];
	char arg[20];
	int i;
	int flags=0;
	struct update_part *fp = f_part;

	memset(cmd, 0 , sizeof(cmd));
	memset(arg, 0 , sizeof(arg));

	sscanf(cmdline, "%s %s", cmd, arg);

	//printf("cmd = %s \n", cmd);
	//printf("arg = %s \n", arg);

	if( strncmp(cmd, "flash", 5)) {
		printf("cmd is not flash \n");
		return -1;
	}

	if( !strncmp(arg,"all", 3)) {
		/* TODO erase all partition */
		printf("flash all \n");
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
			if((fp->partition_name != NULL) &&
				!strcmp(arg, fp->partition_name)) {

				printf("\033[41m flash %d : %s : %s \033[0m\n", i, arg, fp->partition_name);

				printf("fp->device = %s \n", fp->device);

				if(!strncmp(fp->device, "eeprom" , 6) )	{
					/* TODO eeprom erase routine */
                    if (flash_eeprom(file_path, fp) > 0 ) {
                        printf("flash %s finished \n", arg);
                    }

				}
				else if(!strncmp(fp->device, "mmc" , 3) )	{
					if (flash_mmc(file_path, fp) > 0 ) {
						printf("flash %s finished \n", arg);
					} else
						return -1;
				}
				else {
					printf("device is not eeprom or mmc \n");
					return -1;
				}

			}
		}
		else {
               printf("\033[41m flash %d : %s : %s \033[0m\n", i, arg, fp->partition_name);

				printf("fp->device = %s \n", fp->device);

                if(!strncmp(fp->device, "eeprom" , 6) ) {
                    /* TODO eeprom erase routine */
                    if (flash_eeprom(file_path, fp) > 0 ) {
                        printf("flash %s finished \n", arg);
                    }

                }
                else if(!strncmp(fp->device, "mmc" , 3) )   {
                    if (flash_mmc(file_path, fp) > 0 ) {
                        printf("flash %s finished \n", arg);
                    } else
						return -1;
                }
                else {
                    printf("device is not eeprom or mmc \n");
                    return -1;
                }
		}

	}
	return 0;
}

