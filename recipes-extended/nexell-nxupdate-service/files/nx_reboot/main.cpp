#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <linux/reboot.h>
#include <sys/syscall.h>
#include <sys/reboot.h>

#define LOG_TAG "NxReboot"
#include <NX_Log.h>

#define REBOOT_CMD	"recovery"

int main(void)
{
	int ret = 0;

	NXLOGI("Reboot for updating...\n");

	sync();

	ret = syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART2, REBOOT_CMD);
	if (ret != 0) {
		NXLOGE("Reboot error : %d\n", errno);
	}

	return ret;
}
