//------------------------------------------------------------------------------
//
//  Copyright (C) 2016 Nexell Co. All Rights Reserved
//  Nexell Co. Proprietary & Confidential
//
//  NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Name : nxlogrotate
//  Description : Nexell log management daemon.
//  Author : Chris Lee.
//
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <sched.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/inotify.h>

#define NXLOGROTATE_VERSION			"0.1.0"

#define SYSLOG_D_PID        		"/var/run/rsyslogd.pid"
#define SYS_LOG_PATH				"/var/log/syslog"
#define DEBUG_LOG_PATH				"/var/log/debug"
#define USER_LOG_PATH				"/var/log/user.log"
#define MESSAGE_LOG_PATH			"/var/log/messages"

#ifndef SZ_1K
#define SZ_1K						0x00000400
#endif
#ifndef SZ_1M
#define SZ_1M						0x00100000
#endif

#define DEFAULT_LOG_BACKUP_PATH		"/var/log"
#define DEFAULT_SIZE_LIMIT			(SZ_1M * 1)
#define DEFAULT_LOG_ROTATE			5

#define CLEAR_COMMAND				">"

#define EVENT_SIZE					(sizeof(struct inotify_event))
#define PRINT_LEN					1024
#define SYSLOG_LEN					2097
#define BUF_LEN						(PRINT_LEN * (EVENT_SIZE + SYSLOG_LEN))

const char* g_option = "hvs:r:p:";
int32_t g_size_limit = DEFAULT_SIZE_LIMIT;
int32_t g_rotate_count = DEFAULT_LOG_ROTATE;
char g_backupPath[50] = DEFAULT_LOG_BACKUP_PATH;

static void print_usage(void) {
	printf("Usage:\n"
	"  nxlogrotate <option> <value>\n"
	"  ex> nxlogrotate -s 100k -r 5 -p /tmp\n"
	"  ex> nxlogrotate -s 1m -r 5 -p /tmp\n\n"
	"Common options:\n"
	"  -h: help\n"
	"  -v: print versions\n\n"
	"Settings options:\n"
	"  -s: log limitation size (10k ~ 10m), default 1m\n"
	"  -r: log rotation count (0 ~ 10), default 5\n"
	"  -p: backup log path\n\n");
}

static void print_version(void) {
	printf("nxlogrotate version %s\n", NXLOGROTATE_VERSION);
}

static int32_t logrotate_clear(const char *logPath) {
	char command[30];
	sprintf(command, "%s %s", CLEAR_COMMAND, logPath);
	return system(command);
}

static void logrotate(void) {
	int32_t rotate, ret;
	char command[30], copyFile[30], copiedFile[30], symFile[30];

	memset(command, 0, sizeof(command));
	memset(copyFile, 0, sizeof(copyFile));
	memset(copiedFile, 0, sizeof(copiedFile));
	memset(symFile, 0, sizeof(symFile));

	for (rotate = g_rotate_count; rotate > 1; rotate--) {
		// Copy
		sprintf(copiedFile, "%s/syslog.%d", g_backupPath, rotate);
		sprintf(copyFile, "%s/syslog.%d", g_backupPath, rotate - 1);
		if (access(copyFile, F_OK) == 0) {
			sprintf(command, "cp -av %s %s", copyFile, copiedFile);
			ret = system(command);
			if (!ret) {
				// Symbolic link
				if (strcmp(g_backupPath, DEFAULT_LOG_BACKUP_PATH)) {
					sprintf(symFile, "%s.%d", SYS_LOG_PATH, rotate);
					if (access(symFile, F_OK) < 0) {
						memset(command, 0, sizeof(command));
						sprintf(command, "ln -s %s %s.%d", copiedFile, SYS_LOG_PATH, rotate);
						system(command);
					}
				}
			}

			memset(command, 0, sizeof(command));
			memset(copyFile, 0, sizeof(copyFile));
			memset(copiedFile, 0, sizeof(copiedFile));
			memset(symFile, 0, sizeof(symFile));
		}
	}

	// Copy
	sprintf(copiedFile, "%s/syslog.%d", g_backupPath, rotate);
	sprintf(command, "cp -av %s %s", SYS_LOG_PATH, copiedFile);
	ret = system(command);
	if (!ret) {
		// Symbolic link
		if (strcmp(g_backupPath, DEFAULT_LOG_BACKUP_PATH)) {
			sprintf(symFile, "%s.%d", SYS_LOG_PATH, rotate);
			if (access(symFile, F_OK) < 0) {
				memset(command, 0, sizeof(command));
				sprintf(command, "ln -s %s %s.%d", copiedFile, SYS_LOG_PATH, rotate);
				system(command);
			}
		}
	}

    logrotate_clear(SYS_LOG_PATH);
    logrotate_clear(DEBUG_LOG_PATH);
    logrotate_clear(USER_LOG_PATH);
    logrotate_clear(MESSAGE_LOG_PATH);
	sync();
}

static void prevLogBackup(void) {
	int32_t rotate;
	char command[30], prevFile[30], backupFile[30];

	// Remove old backup log files
	memset(command, 0, sizeof(command));
	sprintf(command, "rm -rf %s/syslog.*.old", g_backupPath);
	system(command);

	if (strcmp(g_backupPath, DEFAULT_LOG_BACKUP_PATH)) {
		for (rotate = g_rotate_count; rotate > 0; rotate--) {
			// Move
			memset(prevFile, 0, sizeof(prevFile));
			sprintf(prevFile, "%s/syslog.%d", g_backupPath, rotate);
			if (access(prevFile, F_OK) == 0) {
				memset(command, 0, sizeof(command));
				memset(backupFile, 0, sizeof(backupFile));
				sprintf(backupFile, "%s.old", prevFile);
				sprintf(command, "mv %s %s", prevFile, backupFile);
				system(command);
			}
		}
	}
	sync();
}

int32_t main(int32_t argc, char *argv[]) {
	FILE *hFile = NULL;
	int32_t opt;
	int32_t iNotify = -1;
	int32_t iWatch = -1;
	char readEvent[BUF_LEN];
	char sizeUnit;
	char *size;
	struct sched_param param;
	pid_t pid;

	memset(&param, 0, sizeof(param));
	pid = getpid();
	sched_setscheduler(pid, SCHED_BATCH, &param);

	while ((opt = getopt(argc, argv, g_option)) != -1) {
		switch(opt) {
			case 'h':
			case '?':
				print_usage();
				return 0;
			case 'v':
				print_version();
				return 0;
			case 's':
				sizeUnit = optarg[strlen(optarg)-1];
				if (sizeUnit == 'k' || sizeUnit == 'K') {
					size = strtok(optarg, "kK");
					g_size_limit = (atoi(size) < 10 || atoi(size) > 10240) ? DEFAULT_SIZE_LIMIT : SZ_1K * atoi(size);
				} else if (sizeUnit == 'm' || sizeUnit == 'M') {
					size = strtok(optarg, "mM");
					g_size_limit = (atoi(size) < 1 || atoi(size) > 10) ? DEFAULT_SIZE_LIMIT : SZ_1M * atoi(size);
				}
				break;
			case 'r':
				g_rotate_count = atoi(optarg);
				if (g_rotate_count > 10) {
					g_rotate_count = DEFAULT_LOG_ROTATE;
				}
				break;
			case 'p':
				memset(g_backupPath, 0, sizeof(g_backupPath));
				if (optarg[strlen(optarg)-1] == '/') {
					snprintf(g_backupPath, strlen(optarg), optarg);
				} else {
					snprintf(g_backupPath, strlen(optarg) + 1, optarg);
				}
				break;
			default:
				printf("getopt returned character code 0%o\n", opt);
		}
	}

	if (optind < argc) {
		while (optind < argc) {
			optind += 1;
		}
	}

    // Check if syslog daemon is running
    if (access(SYSLOG_D_PID, F_OK) < 0) {
        printf("Please check if syslog daemon is running.\n");
        goto ERROR;
    }

	if ((hFile = fopen(SYS_LOG_PATH, "r")) == NULL) {
		printf("Fail, fopen(). (%s)\n", strerror(errno));
		printf("Can't find syslog file. (%s)\n"
		"Please check if syslog daemon is running.\n", SYS_LOG_PATH);
		goto ERROR;
	}

	iNotify = inotify_init();
	if (iNotify < 0) {
		printf("Fail, inotify_init(). (%s)\n", strerror(errno));
		goto ERROR;
	}

	iWatch = inotify_add_watch(iNotify, SYS_LOG_PATH, IN_MODIFY | IN_DELETE_SELF | IN_MOVE_SELF);
	if (iWatch < 0) {
		printf("Fail, inotify_add_watch(). (%s)\n", strerror(errno));
		goto ERROR;
	}

	// If it exists, back up the previous log files before the log rotation
	prevLogBackup();

	while (1) {
		struct pollfd hPoll;
		hPoll.fd		= iNotify;
		hPoll.events	= POLLIN | POLLERR;
		hPoll.revents	= 0;
		int32_t ret = poll((struct pollfd *)&hPoll, 1, 1000);
		if (ret < 0) {
			printf("Fail, poll(). (%s)\n", strerror(errno));
			break;
		} else if (ret == 0) {
			continue;
		}

		if (hPoll.revents & POLLIN) {
			int32_t readSize = read(iNotify, readEvent, BUF_LEN);
			int32_t eventPos = 0;
			if (readSize < 0) {
				printf("Fail, read(). (%s)\n", strerror(errno));
				break;
			}

			// Detect notification event
			while (eventPos < readSize) {
				struct inotify_event *pNotifyEvent = (struct inotify_event *)&readEvent[eventPos];
				if (pNotifyEvent->mask & IN_MODIFY) {
					// Check file size
					if ((hFile = fopen(SYS_LOG_PATH, "r")) == NULL) {
						printf("Fail, fopen(). (%s)\n", strerror(errno));
						printf("Can't find syslog file. (%s)\n"
						"Please check if syslog daemon is running.\n", SYS_LOG_PATH);
						break;
					}

					fseek(hFile, 0, SEEK_END);
					int32_t fileLength = ftell(hFile);
					fclose(hFile);

					if (fileLength > g_size_limit) {
						if (g_rotate_count == 0) {
							logrotate_clear(SYS_LOG_PATH);
							logrotate_clear(DEBUG_LOG_PATH);
							logrotate_clear(USER_LOG_PATH);
							logrotate_clear(MESSAGE_LOG_PATH);
							sync();
						} else {
							logrotate();
						}
					}

				} else if (pNotifyEvent->mask & IN_DELETE_SELF || pNotifyEvent->mask & IN_MOVE_SELF) {
					break;
				}
				eventPos += EVENT_SIZE + pNotifyEvent->len;
			}
		}
	}

ERROR:
	if (iWatch > 0)
		inotify_rm_watch(iNotify, iWatch);
	if (iNotify > 0)
		close(iNotify);
	if (hFile)
		fclose(hFile);

	return 0;
}
