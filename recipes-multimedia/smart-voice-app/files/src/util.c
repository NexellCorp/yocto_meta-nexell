#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "util.h"
#include "util_debug.h"

#ifdef SUPPORT_RT_SCHEDULE
#include <sched.h>
#include <sys/resource.h>
#include <linux/sched.h> /* SCHED_NORMAL, SCHED_FIFO, SCHED_RR, SCHED_BATCH */

/* sched_new_scheduler(getpid(), SCHED_FIFO, 99) */
static int sched_new_scheduler(pid_t pid, int policy, int priority)
{
	struct sched_param param;
	int maxpri = 0, minpri = 0;
	int ret;

	switch (policy) {
	case SCHED_NORMAL:
	case SCHED_FIFO:
	case SCHED_RR:
	case SCHED_BATCH:
		break;
	default:
		pr_err("invalid policy %d (0~3)\n", policy);
		return -1;
	}

	if (policy == SCHED_NORMAL) {
		/* #define NICE_TO_PRIO(nice)
		 * (MAX_RT_PRIO + (nice) + 20), MAX_RT_PRIO 100
		 */
		maxpri =  20;
		minpri = -20;	/* nice */
	} else {
		maxpri = sched_get_priority_max(policy);
		minpri = sched_get_priority_min(policy);
	}

	if ((priority > maxpri) || (minpri > priority)) {
		pr_err("E: invalid priority (%d ~ %d)...\n", minpri, maxpri);
		return -1;
	}

	if (policy == SCHED_NORMAL) {
		param.sched_priority = 0;
		ret = sched_setscheduler(pid, policy, &param);
		if (ret) {
			pr_err("E: set scheduler (%d) %s\n"
				ret, strerror(errno));
			return ret;
		}
		ret = setpriority(PRIO_PROCESS, pid, priority);
		if (ret)
			pr_err("E: set priority (%d) %s\n",
				ret, strerror(errno));
	} else {
		param.sched_priority = priority;
		ret = sched_setscheduler(pid, policy, &param);
		if (ret)
			pr_err("E: set scheduler (%d) %s\n",
				ret, strerror(errno));
	}
	return ret;
}
#endif

pid_t gettid(void)
{
	return syscall(__NR_gettid);
}

#include <execinfo.h>

#define BT_ARRAY_SIZE	256
void sig_handler(int sig)
{
	void *array[BT_ARRAY_SIZE];
	size_t size;

	size = backtrace(array, BT_ARRAY_SIZE);
	fprintf(stderr, "\n\nError: signal %d:\n", sig);
	/* #include <execinfo.h> */
	backtrace_symbols_fd(array, size, 2);

	signal(SIGSEGV, SIG_DFL);
	raise(SIGSEGV);
}

int sys_write(const char *file, const char *buffer, int buflen)
{
	int fd, ret = 0;

	if (access(file, F_OK)) {
		pr_err("E: cannot access file (%s).\n", file);
		return -errno;
	}

	fd = open(file, O_RDWR|O_SYNC);
	if (fd < 0) {
		pr_err("E: cannot open file (%s).\n", file);
		return -errno;
	}

	ret = write(fd, buffer, buflen);
	if (ret < 0) {
		pr_err("E: write (file=%s, data=%s)\n", file, buffer);
		ret = -errno;
	}

	close(fd);
	return ret;
}

int sys_read(const char *file, char *buffer, int buflen)
{
	int fd, ret = 0;

	if (access(file, F_OK)) {
		pr_err("E: cannot access file (%s).\n", file);
		return -errno;
	}

	fd = open(file, O_RDONLY);
	if (fd < 0) {
		pr_err("E: cannot open file (%s).\n", file);
		return -errno;
	}

	ret = read(fd, buffer, buflen);
	if (ret < 0) {
		pr_err("E: read (file=%s, data=%s)\n", file, buffer);
		ret = -errno;
	}

	close(fd);
	return ret;
}

#define CPU_FREQUENCY_PATH	"/sys/devices/system/cpu"
int cpu_set_frequency(long khz)
{
	char path[128], data[32];
	int ret;

	/*
	 * change governor to userspace
	 */
	sprintf(path,
		"%s/cpu0/cpufreq/scaling_governor", CPU_FREQUENCY_PATH);
	ret = sys_write(path, "userspace", strlen("userspace"));
	if (ret < 0)
		return ret;

	/*
	 *	change cpu frequency
	 */
	sprintf(path,
		"%s/cpu0/cpufreq/scaling_setspeed", CPU_FREQUENCY_PATH);
	sprintf(data, "%ld", khz);
	ret = sys_write(path, data, strlen(data));
	if (ret < 0)
		return ret;

	return 0;
}

long cpu_get_frequency(void)
{
	char path[128], data[32];
	int ret;

	sprintf(path, "%s/cpu0/cpufreq/scaling_cur_freq", CPU_FREQUENCY_PATH);
	ret = sys_read(path, data, sizeof(data));
	if (ret < 0)
		return ret;

	return strtol(data, NULL, 10);	 /* khz */
}

