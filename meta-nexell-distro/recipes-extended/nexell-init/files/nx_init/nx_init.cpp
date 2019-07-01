#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mount.h>

#define NXQUICKREARCAM
#undef NXQUICKSVM

#ifdef NXQUICKREARCAM
#include <linux/fs.h>
#include <fcntl.h>
#include <string.h>

#define CMDLINE_READ_SIZE 512
#define OPTION_CNT_MAX 16

struct nx_cam_option {
	int option;
	char buffer[32];
};

struct option_string {
	char string[32];
};

static struct option_string string[] = {
	{"nx_cam.m="},
	{"nx_cam.g="},
	{"nx_cam.b="},
	{"nx_cam.c="},
	{"nx_cam.v="},
	{"nx_cam.p="},
	{"nx_cam.r="},
	{"nx_cam.d="},
	{"nx_cam.l="},
	{"nx_cam.s="},
	{"nx_cam.D="},
	{"nx_cam.R="},
	{"nx_cam.P="},
	{"nx_cam.L="},
	{"nx_cam.end"},
};

static int search(const char *str_opt, const char *src, char *dest)
{
	const char *pos, *pos1;
	int len;
	int opt_len;

	if (strncmp(str_opt, "nx_cam.end", strlen("nx_cam.end")) == 0)
		return -1;

	pos = strstr(src, str_opt);
	if (pos) {
		len = strlen(str_opt);
		pos += len;

		pos1 = strstr(pos, " ");
		opt_len = pos1 - pos;

		strncpy(dest, pos, opt_len);

		return 0;
	}

	return -1;
}

static int search_option(const char *src, struct nx_cam_option *option)
{
	int i;
	int string_cnt = sizeof(string)/sizeof(struct option_string);
	int opt_cnt = 0;
	char tmp_buffer[32];

	if (string_cnt > OPTION_CNT_MAX) {
		string_cnt = OPTION_CNT_MAX;
		printf("Please check the MAX count of option:%s\n", OPTION_CNT_MAX);
	}

	if (search("nx_cam.", src, tmp_buffer) != 0)
		return opt_cnt;

	for (i = 0; i < string_cnt; i++) {
		if (search(string[i].string, src, option->buffer) == 0) {
			/* printf("option->buffer:%s\n", option->buffer); */
			option->option = 1;
			option++;
			opt_cnt++;
		}
	}

	return opt_cnt;
}
#endif

int main(int argc, char *argv[])
{
	pid_t pid;
	int status = 0;
	int access_ret = 0;

	mount("sysfs", "/sys", "sysfs", 0, NULL);
#ifdef NXQUICKREARCAM
	mount("proc", "/proc", "proc", 0, NULL);
#endif
#ifdef NXQUICKSVM
	mkdir("/svmdata", 0755);
	mount("/dev/mmcblk0p2", "/svmdata", "ext4", 0, NULL);
#endif

	pid = fork();

	switch(pid)
	{
		case -1:
		{
			printf("fail create child process \n");
			return -1;
		}

		case 0: {
#ifdef NXQUICKREARCAM
			struct nx_cam_option cam_option[OPTION_CNT_MAX];
			int fd = 0;
			int r_size = 0;
			int opt_cnt = 0;
			char cmdline[CMDLINE_READ_SIZE];

			access_ret = access("/sbin/NxQuickRearCam", 0);
			if (access_ret == 0) {
				memset((void*)cam_option, 0x0, sizeof(struct nx_cam_option) * OPTION_CNT_MAX);
				opt_cnt = 0;

				fd = open("/proc/cmdline", O_RDONLY);
				if (fd >= 0) {
					r_size = read(fd, cmdline, CMDLINE_READ_SIZE);
					if (r_size == CMDLINE_READ_SIZE)
						printf("Please check the read size of cmdline!!!\n");
					opt_cnt = search_option(cmdline, cam_option);
					close(fd);
				} else {
					printf("/proc/cmdline open fail:%d \n", fd);
				}

				if (opt_cnt == 0) {
					printf("start NxQuickRearCam -m1 -b1 -c26 -r704x480 (default)\n");
					execl("/sbin/NxQuickRearCam", "NxQuickRearCam", "-m1", "-b1", "-c26", "-r704x480", NULL);
				} else {
					printf("start NxQuickRearCam %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n"
						, cam_option[0].buffer, cam_option[1].buffer, cam_option[2].buffer, cam_option[3].buffer, cam_option[4].buffer
						, cam_option[5].buffer, cam_option[6].buffer, cam_option[7].buffer, cam_option[8].buffer, cam_option[9].buffer
						, cam_option[10].buffer, cam_option[11].buffer, cam_option[12].buffer, cam_option[13].buffer, cam_option[14].buffer
						, cam_option[15].buffer);
					execl("/sbin/NxQuickRearCam", "NxQuickRearCam"
						, cam_option[0].buffer, cam_option[1].buffer, cam_option[2].buffer, cam_option[3].buffer, cam_option[4].buffer
						, cam_option[5].buffer, cam_option[6].buffer, cam_option[7].buffer, cam_option[8].buffer, cam_option[9].buffer
						, cam_option[10].buffer, cam_option[11].buffer, cam_option[12].buffer, cam_option[13].buffer, cam_option[14].buffer
						, cam_option[15].buffer);
				}
			}
#endif
#ifdef NXQUICKSVM
			printf("start nx_3d_avm_daemon \n");
			execl("/sbin/nx_3d_avm_daemon", "nx_3d_avm_daemon", NULL);
#endif
			break;
		}

		default:
		{
			usleep(500000);
			printf("start systemd \n");
			execl("/lib/systemd/systemd","systemd", NULL);
			break;
		}
	}

	waitpid(pid, &status, 0);

	return 0;
}
