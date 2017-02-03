#ifndef _UTIL_H_
#define _UTIL_H_

#include <signal.h>

#if __cplusplus
extern "C" {
#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define	MSLEEP(m)	usleep(m*1000)

#define	RUN_TIMESTAMP_US(s) {		\
		struct timeval tv;	\
		gettimeofday(&tv, NULL);	\
		s = (tv.tv_sec*1000000) + (tv.tv_usec);	\
	}

#define	END_TIMESTAMP_US(s, d)	{	\
		struct timeval tv;	\
		gettimeofday(&tv, NULL);	\
		d = (tv.tv_sec*1000000) + (tv.tv_usec);	\
		d = d - s;	\
	}

struct list_entry {
	struct list_entry *next, *prev;
	void *data;
};
#define LIST_HEAD_INIT(name, data) { &(name), &(name), data }
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

static inline void INIT_LIST_HEAD(struct list_entry *list, void *data)
{
	list->next = list;
	list->prev = list;
	list->data = data;
}

static inline void __list_add(struct list_entry *list,
			struct list_entry *prev, struct list_entry *next)
{
	next->prev = list, list->next = next;
	list->prev = prev, prev->next = list;
}

/* add to tail */
static inline void list_add(struct list_entry *list, struct list_entry *head)
{
	__list_add(list, head->prev, head);
}

#ifdef SUPPORT_RT_SCHEDULE
/* sched_new_scheduler(getpid(), SCHED_FIFO, 99) */
int sched_new_scheduler(pid_t pid, int policy, int priority);
#endif

pid_t gettid(void);

void sig_handler(int sig);
int sys_write(const char *file, const char *buffer, int buflen);
int sys_read(const char *file, char *buffer, int buflen);

#define CPU_FREQUENCY_PATH	"/sys/devices/system/cpu"
int cpu_set_frequency(long khz);

long cpu_get_frequency(void);

#if __cplusplus
}
#endif
#endif /* _UTIL_H_ */
