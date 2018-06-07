#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <unistd.h>
#include    "def.h"
#include	"fdisk.h"

extern int max_part;
extern struct update_part f_part[UPDATE_DEV_PART_MAX];
extern int check_mmc_f_part(struct update_part *f_part);
extern int is_mounted_mmc (void);
extern unsigned long long int get_mmc_blk_size();
#ifdef  PSPLASH_UI
extern int print_msg(char * msg);
extern int print_progress(unsigned int msg);
#endif

static int saturate_fdisk_table(struct update_part *f_part, struct sfdisk_part * sf_part, unsigned long long total_blk) {
    int i=0;
    struct update_part *fp = f_part;
	struct sfdisk_part *sfp = sf_part;
    unsigned long long ext_start_addr=0;
    unsigned long long ext_size=0;


    for(i=0 ; i<UPDATE_DEV_PART_MAX; i++, fp++) {

		if(fp->part_num == 0) continue; //skip the raw area

		if(fp->part_num == 3) {
			ext_start_addr = fp->start + fp->length;
			ext_size = (total_blk * 2 * BLK_SIZE ) - (ext_start_addr);
		}
		// Add extend partition
		if(fp->part_num == 5) {
			sfp->start_sector = ext_start_addr / BLK_SIZE;
			sfp->sector_size = ext_size / BLK_SIZE;
			sfp->id = 5; //extend
			sfp++;
		}

		if(fp->part_num == max_part) {
			fp->length = (total_blk * 2 * BLK_SIZE ) - (fp->start);
		}

		sfp->start_sector = fp->start / BLK_SIZE;
		sfp->sector_size = fp->length / BLK_SIZE;
		sfp->id = 83; //Linux
		sfp++;
    }
    return 0;

}

int do_fdisk(char * cmdline) {

	int i=0;
	struct update_part *fp = f_part;
	struct sfdisk_part sf_part[UPDATE_DEV_PART_MAX];
	unsigned long long total_blk;
	char sfdisk_cmd[128];
	char sfdisk_buf[1024];
	char sfdisk_tmp[128];
	char file_path[128];
	char dd_cmd[128];
	FILE *sfdisk_temp_file;

	//printf("%s : Enter ++ \n", __FUNCTION__);
	printf("\033[41m fdisk \033[0m\n");
	#ifdef  PSPLASH_UI
	print_msg("Updates fdisk :");
	//print_progress(10);
	#endif
	//1. Check if need the Fdisk (f_part has mmc?)
	if ( !check_mmc_f_part(fp) )
		return -1;
	// check mmcblk0 mounted and umount
	if(is_mounted_mmc()) {
        //umount mmcblk0*
        if (system("/bin/umount /dev/mmcblk0*") <  0) {
                printf("sh: umount error \n");
        }

	}

	//2. Get mmc capacity size
	total_blk = get_mmc_blk_size();
	if(total_blk < 0) {
		printf("mmc sector size read error \n");
		exit(-1);
	}

	//3. Saturate fdisk partition table
	saturate_fdisk_table(fp, sf_part, total_blk);
	/*
	for(i=0;i<max_part;i++)	{
		printf("i = %d , start_sector = %lld , sector_size = %lld , id = %d \n",
				i+1, sf_part[i].start_sector, sf_part[i].sector_size, sf_part[i].id);
	}
	*/
	//4. execute the sfdisk command
	memset(file_path,0,sizeof(file_path));
	memset(sfdisk_buf,0,sizeof(sfdisk_buf));
	memset(sfdisk_tmp,0,sizeof(sfdisk_tmp));
	memset(sfdisk_cmd,0,sizeof(sfdisk_cmd));
	memset(dd_cmd,0,sizeof(dd_cmd));

	sprintf(file_path, "/tmp/%s",SFDISK_TEMP);
	sfdisk_temp_file = fopen(file_path,"w");

	sprintf(sfdisk_cmd,"sfdisk -f -uS %s  < %s", MMC_DEV_NAME, file_path);

	for(i=0;i<max_part;i++) {
		sprintf(sfdisk_tmp,"%lld, %lld, %d\n",
			sf_part[i].start_sector, sf_part[i].sector_size, sf_part[i].id);
		strncat(sfdisk_buf, sfdisk_tmp, strlen(sfdisk_tmp));
		printf("%s\n", sfdisk_tmp);
	}
	strncat(sfdisk_buf, "\n", 1);

	fwrite(sfdisk_buf, 1, strlen(sfdisk_buf), sfdisk_temp_file);

	fclose(sfdisk_temp_file);


	sprintf(dd_cmd, "dd if=/dev/zero of=%s bs=512 count=1", MMC_DEV_NAME);

	//Erase partition table
	if (system(dd_cmd) <  0) {
                printf("sh: %s error \n", dd_cmd);
	}

	if (system(sfdisk_cmd) <  0) {
                printf("sh: %s error \n", sfdisk_cmd);
	}
	#ifdef  PSPLASH_UI
	print_msg("Updates fdisk : Finished");
	//print_progress(100);
	#endif

	return 0;
}

