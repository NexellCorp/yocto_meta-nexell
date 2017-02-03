#ifndef _EVENT_H_
#define _EVENT_H_

#include <unistd.h>
#include <signal.h>

#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <linux/netlink.h>

struct udev_message {
	const char *sample_frame;
	int sample_rate;
};

static int audio_event_init(void)
{
	struct sockaddr_nl addr;
	int sz = 64*1024;
	int fd;

	memset(&addr, 0, sizeof(addr));
	addr.nl_family = AF_NETLINK;
	addr.nl_pid = getpid();
	addr.nl_groups = 0xffffffff;

	fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (fd < 0)
		return -EINVAL;

	setsockopt(fd, SOL_SOCKET, SO_RCVBUFFORCE, &sz, sizeof(sz));
	if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		close(fd);
		return -EINVAL;
	}

	return fd;
}

static void audio_event_close(int fd)
{
	close(fd);
}

static int audio_event_msg(int fd, char *buffer, int length)
{
	struct pollfd pfd;
	int n, count;

	do {
		pfd.fd = fd;
		pfd.events = POLLIN;
		pfd.revents = 0;
		n = poll(&pfd, 1, -1);
		if (n > 0  && (POLLIN & pfd.revents)) {
			count = recv(fd, buffer, length, 0);
			if (count > 0)
				return count;
		}
	} while (1);

	/* won't get here */
	return 0;
}
#endif /* _EVENT_H_ */
