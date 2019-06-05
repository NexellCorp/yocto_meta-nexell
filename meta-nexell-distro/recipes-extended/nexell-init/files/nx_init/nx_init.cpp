#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mount.h>

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
			access_ret = access("/sbin/NxQuickRearCam", 0);
			if (access_ret == 0)
				execl("/sbin/NxQuickRearCam", "NxQuickRearCam", "-m1", "-b1", "-c26", "-r704x480", NULL);
			break;
		}

		default:
		{
			execl("/lib/systemd/systemd","systemd", NULL);
			break;
		}
	}

	waitpid(pid, &status, 0);

	return 0;
}
