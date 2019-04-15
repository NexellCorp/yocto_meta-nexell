#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mount.h>

#define NXQUICKREARCAM
#undef NXQUICKSVM

int main(int argc, char *argv[])
{
	pid_t pid;
	int status = 0;
	int access_ret = 0;

	mount("sysfs", "/sys", "sysfs", 0, NULL);
	mkdir("/svmdata", 0755);
	mount("/dev/mmcblk0p2", "/svmdata", "ext4", 0, NULL);

	pid = fork();

	switch(pid)
	{
		case -1:
		{
			printf("fail create child process \n");
			return -1;
		}

		case 0:
		{
#ifdef NXQUICKREARCAM
			access_ret = access("/sbin/NxQuickRearCam", 0);
			if (access_ret == 0){
				printf("start NxQuickRearCam \n");
				execl("/sbin/NxQuickRearCam", "NxQuickRearCam", "-m1", "-b1", "-c26", "-r704x480", NULL);
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
