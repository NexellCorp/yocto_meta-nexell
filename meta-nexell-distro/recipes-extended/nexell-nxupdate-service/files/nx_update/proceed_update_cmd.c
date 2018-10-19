#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <unistd.h>
#include	"def.h"
#include    "proceed_update_cmd.h"
#include    "erase.h"
#include    "flash.h"
#include    "fdisk.h"

#define UPDATE_CMD      "update_cmd.txt"
#ifdef  PSPLASH_UI
extern int print_msg(char * msg);
extern int print_progress(unsigned int msg);
#endif


typedef struct _CMD_STR {
	unsigned int cnt;
	char command[100][100];
} CMD_STR;

CMD_STR		update_cmd_str;

static int parse_comment(char *update_str) {

	char *token = NULL;
	char comment_char[] = "\n";
	char *str = update_str;
	//int i;

	token = strtok( str, comment_char );
	update_cmd_str.cnt = 0;


    while( token != NULL )  {

        if (*token != '#') {
            strncpy(update_cmd_str.command[update_cmd_str.cnt], token, strlen(token));
            update_cmd_str.cnt++;
        }
        token = strtok( NULL, comment_char );
    }
	/*
	for(i=0; i < update_cmd_str.cnt ; i++)
		 printf("cmd[%d] = %s \n",i, update_cmd_str.command[i]);
	*/

	return 0;

}

extern struct update_part f_part[UPDATE_DEV_PART_MAX];

static int execute_command(char *file_path) {

	int i=0;
	//int progress=0;

	for(i=0; i< update_cmd_str.cnt ; i++){

		//print_progress(progress);

		if(!strncmp(update_cmd_str.command[i], "fdisk", 5)) {
			if(do_fdisk(update_cmd_str.command[i])<0) {
				printf("fdisk command failed \n");
				print_msg("Update failed : Error code = 1003");
				return -1;
			}
		}
		else if (!strncmp(update_cmd_str.command[i], "erase", 5)) {
			if (do_erase(update_cmd_str.command[i]) < 0) {
				print_msg("Update failed : Error code = 1004");
				return -1;
			}

		}
		else if (!strncmp(update_cmd_str.command[i], "flash", 5)){
			if (do_flash(file_path, update_cmd_str.command[i]) < 0) {
				print_msg("Update failed : Error code = 1005");
				return -1;
			}
		}
		else {
			//others
			if (system(update_cmd_str.command[i]) <  0) {
				printf("sh: %s error \n", update_cmd_str.command[i]);
				print_msg("Update failed : Error code = 1002");
				return -1;
			}
		}
	}
	return 0;
}

int proceed_update_cmd_sh(char *file_path, struct update_part *f_part)
{
	FILE    *update_cmd;
    char 	*str_cmd;
	size_t	r_size=0;
	char file_name[20];

	memset(file_name, 0 , sizeof(file_name));

	sprintf(file_name,"%s/%s",file_path, UPDATE_CMD);
	update_cmd = fopen (file_name, "r");

    if(update_cmd) {
        struct stat buf;
        stat(file_path, &buf);
        str_cmd = (char *) malloc (buf.st_size * sizeof(char));
        r_size = fread(str_cmd, 1, buf.st_size, update_cmd);
        if(r_size > 0) {
			//cmd seperated ;
			parse_comment(str_cmd);
			if(execute_command(file_path) < 0) {
				fclose(update_cmd);
				free(str_cmd);
				return -1;
			}
        }
    }
    else {
        printf("File %s open Failed \n", file_path );
		print_msg("update failed : Error code = 1001");
        return -1;
    }

	fclose(update_cmd);
	free(str_cmd);

	return 0;
}
