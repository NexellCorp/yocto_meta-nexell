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
	int status;

	//printf("## [%s():%d\t] mount sysfs /sys \n", __FUNCTION__, __LINE__);
	mount("sysfs", "/sys", "sysfs", 0, NULL);

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
			//printf("## [%s():%d\t] execl path:/sbin/NxQuickRearCam -m1 -b1 -c26 -r960x480 \n", __FUNCTION__, __LINE__);
			execl("/sbin/NxQuickRearCam", "NxQuickRearCam", "-m1", "-b1", "-c26", "-r960x480", NULL);
			break;
		}

		default:
		{
			//printf("## [%s():%d\t] execl path:/lib/systemd/systemd \n", __FUNCTION__, __LINE__);
			execl("/lib/systemd/systemd","systemd", NULL);
			break;
		}
	}

	waitpid(pid, &status, 0);

	return 0;
}
