#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <unistd.h>
#include 	<fcntl.h>

#include    "def.h"

extern int max_tbl_part;

int check_mmc_f_part(struct update_part *f_part) {

    int i=0;
    struct update_part *fp = f_part;

    for(i=0 ; i<max_tbl_part; i++, fp++) {
        if(!strncmp(fp->device,"mmc",3)) return 1;
    }
    return 0;
}

int is_mounted_mmc (void){

	FILE * proc_mount;
	char    file_path[100];
	char 	cmd[100];
	char *p;
    size_t r_size=0;
	int ret=0;


	sprintf(cmd,"cat %s > %s",PROC_MOUNT, PROC_TEMP);

	if ( system(cmd) < 0) {
		printf("sh: %s error \n", cmd);
	}

	if ( system("sync") < 0) {
		printf("sh: sync error \n");
	}

	sprintf(file_path,"%s",PROC_TEMP);

    proc_mount = fopen (file_path, "r");

    if(proc_mount) {
        struct stat buf;
		char * strret=NULL;
        stat(file_path, &buf);
        p = (char *) malloc (buf.st_size * sizeof(char));
        r_size = fread(p, 1, buf.st_size, proc_mount);
        if(r_size > 0) {
			strret = strstr(p, "mmcblk0");
			if(strret != NULL)
				ret = 1;
			else
				ret = 0;
        }
    }
    else {
        printf("File %s open Failed \n", file_path );
        exit(-1);
    }

    fclose(proc_mount);
	free(p);
	return ret;
}

unsigned long long int get_mmc_blk_size() {
    char cmd[128];
    char    file_path[100];
    unsigned long long sec_size=0;
    FILE *cap_sec;
    char *p;
    size_t r_size=0;

    sprintf(file_path, "/tmp/%s",MMC_TEMP);

    sprintf(cmd,"sfdisk -s %s > %s", MMC_DEV_NAME, file_path);

    if( system(cmd) < 0 ) {
        printf("sh: %s error \n",cmd);
    }

    if ( system("sync") < 0 ) {
        printf("sh: sync error \n");
    }

    cap_sec = fopen(file_path,"r");

    if(cap_sec) {
        struct stat buf;
        stat(file_path, &buf);
        p = (char *) malloc (buf.st_size * sizeof(char));
        r_size = fread(p, 1, buf.st_size, cap_sec);
        if(r_size > 0)
            sec_size = strtoul(p, NULL, 10);
    }
    else {
        printf("File %s open Failed \n", file_path );
        exit(-1);
    }

	printf("sec_size = %lld \n",sec_size);

    fclose(cap_sec);

    return sec_size;
}
#ifdef	PSPLASH_UI
static int psplash_write(char *cmd);

int print_msg(char * msg)
{
	char cmd[128];
	memset(cmd, 0, sizeof(cmd));
	//sprintf(cmd,"%s \"MSG %s\"", PSPLASH, msg);
	sprintf(cmd,"MSG %s ", msg);
	//if( system(cmd) < 0 ) {
	if( psplash_write(cmd) < 0 ) {
        printf("sh: %s error \n",cmd);
    }
	return 1;
}

int print_progress(unsigned int msg)
{
	char cmd[128];

	memset(cmd, 0, sizeof(cmd));
	//sprintf(cmd,"%s \"PROGRESS %d\"", PSPLASH, msg);
	sprintf(cmd,"PROGRESS %d \n", msg);

	//if( system(cmd) < 0 ) {
	if( psplash_write(cmd) < 0 ) {
        printf("sh: %s error \n",cmd);
    }
	return 1;
}

#define PSPLASH_FIFO "/tmp/psplash_fifo"

int   pipe_fd;

int psplash_init() {

	if ((pipe_fd = open (PSPLASH_FIFO,O_WRONLY|O_NONBLOCK)) == -1)
    {
      /* Silently error out instead of covering the boot process in
         errors when psplash has exitted due to a VC switch */
      /* perror("Error unable to open fifo"); */
      return (-1);
    }
	return 0;

}

int psplash_close() {

	close(pipe_fd);
	return 0;
}


static int psplash_write(char *cmd)
{
	//printf("cmd = %s ", cmd);

	write(pipe_fd, cmd, strlen(cmd)+1);

	return 1;
}

#endif

void DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i=0, j;
	ascii[16] = '\0';
	printf("0x%08X : ",i);
	for (i = 0; i < size; ++i) {

		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|  %s \n", ascii);
				printf("0x%08X : ",i+1);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
				printf("0x%08X : ",i+1);
			}
		}
	}

	printf("\n");
}
