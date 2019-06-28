#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sched.h>
#include <signal.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <pthread.h>
#include <time.h>
#include <sys/stat.h>
#include <asm/unistd.h>

#ifndef BOOTTIME_LAUNCHER
#define BOOTTIME_LAUNCHER 0 // us
#endif

#define	LAUNCHER_CONF	"/etc/qboot/launcher.conf"

//#define CHK_BOOTTIME
//#define ENABLE_CONSOLE
//#define ENABLE_PRELOAD

/**
 * launcher_t:
 */
typedef struct {
	char *s;
	char d[512];
	bool found;
	bool isdir;
	bool check;
} launcher_t;

/*
 * for: /etc/qboot/launcher.conf
 */
#define RUN_LAUNCHER		"BOOT_LAUNCHER"
#define RUN_LAUNCHER_ARGS	"BOOT_LAUNCHER_ARGS"

#ifdef TSLIB
#define SET_TSDEVICE		"TSLIB_TSDEVICE"
#define SET_TSCAL			"TSLIB_CALIBFILE"
#define SET_TSPLUG			"TSLIB_PLUGINDIR"
#define SET_PCAL			"POINTERCAL_FILE"
#endif

#define SET_LANG			"LANG"

static launcher_t launcher_info[] = {
	{ .s = RUN_LAUNCHER, },
	{ .s = RUN_LAUNCHER_ARGS, },
#ifdef TSLIB
	{ .s = "TSLIB_TSDEVICE", },
	{ .s = "TSLIB_CALIBFILE", },
	{ .s = "TSLIB_PLUGINDIR", },
	{ .s = "POINTERCAL_FILE", },
#endif
	{ .s = "QT_QPA_PLATFORM", },
#if defined (TSLIB) && defined (EGLFS)
	{ .s = "QT_QPA_EGLFS_TSLIB", },
#elif defined (TSLIB) && defined (LINUXFB)
	{ .s = "QT_QPA_FB_TSLIB", },
#endif
	{ .s = "QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS", },
#ifdef EGLFS
	{ .s = "QT_QPA_EGLFS_NO_LIBINPUT", },
	{ .s = "QT_QPA_EGLFS_KMS_CONFIG", },
	{ .s = "QT_QPA_EGLFS_SWAPINTERVAL", },
	{ .s = "QT_QPA_EGLFS_HIDECURSOR", },
#elif defined (LINUXFB)
	{ .s = "QT_QPA_FB_NO_LIBINPUT", },
#endif
	{ .s = "QT_QPA_FONTDIR", },
#ifdef EGLFS
	{ .s = "QT_QPA_EGLFS_DEBUG", },
#endif
	{ .s = "QT_PLUGIN_PATH", },
	{ .s = "QT_LOGGING_RULES", },
	{ .s = "LANG", }
};

/**
 * ARRAY_SIZE:
 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

/**
 * TIME STAMP:
 */
#define	RUN_TIMESTAMP_US(s) { \
		struct timespec tv; \
		clock_gettime(CLOCK_BOOTTIME, &tv); \
		s = (tv.tv_sec * 1000000) + (tv.tv_nsec / 1000); \
}

#define	END_TIMESTAMP_US(s, d) { \
		struct timespec tv; \
		clock_gettime(CLOCK_BOOTTIME, &tv); \
		d = (tv.tv_sec * 1000000) + (tv.tv_nsec / 1000); \
		d = d - s; \
	}

typedef struct {
	long long ts;
	long long td;
} ts_t;

#define SET_TS(s, t)	(((ts_t *)s)->ts = t)
#define RUN_TS(s)		RUN_TIMESTAMP_US(((ts_t *)s)->ts)
#define END_TS(s)		END_TIMESTAMP_US(((ts_t *)s)->ts, ((ts_t *)s)->td)
#define TS_US(s)		(((ts_t *)s)->td)
#define TS_MS(s)		(((ts_t *)s)->td / 1000)

/**
 * message:
 */
#define	__msg__(m...)	printf(m)
static char console[64];

static void message(const char *f, ...) {
	FILE *fd;
	va_list args;
	char tty[512] = {0, };
	char buf[512] = {0, };

	sprintf(tty, "/dev/%s", console);
	fd = fopen(tty, "wb");

	va_start(args, f);
	vsprintf(buf, f, args);
	va_end(args);

	/* out to default console */
	if (!fd) {
		__msg__(buf);
		return;
	}

	/* write to console */
	fwrite(buf, 1, strlen(buf), fd);
	fclose(fd);
}

/**
 * fatal:
 */
#define fatal(format, ...) \
	do { \
		message("Fatal: "); \
		message(format, ##__VA_ARGS__); \
		exit(127); \
	} while (0)

/**
 * error:
 */
#define error(format, ...) \
	do { \
		message("Error : %s:%d: ", __func__, __LINE__); \
		message(format, ##__VA_ARGS__); \
	} while (0)

#ifdef ENABLE_PRELOAD
/**
 * gettid
 */
static pid_t gettid(void) {
        return (pid_t)syscall(__NR_gettid);
}
#endif

/**
 * parse_option - parse option
 * @str : source string
 * @opt : option to search
 * @out : option
 * @eopt : option to line end
 */
static int parse_option(char *str, char *opt, char *out, char *eopt) {
	char *p, *e;
	char buf[256] = {0, };
	int i = 0;
 	bool dquotes = false;	/* " */

	if (*str == '#') {
		return -1;
	}

	if (str[0] == '/' && str[1] == '/') {
		return -1;
	}

	/* search opt with str */
	p = strstr(str, opt);
	if (!p) {
		return -1;
	}

	/* check total opt in case opt:AAA, str:AAA_BBB */
	e = p + strlen(opt);
	if (!isspace((unsigned char)*e) && *e != '=') {
		return -1;
	}

	/* go '=' */
	p = strstr(e, "=");
	if (!p) {
		return -1;
	}

	/* go first char */
	while (p++) {
		if (!isspace((unsigned char)*p)) {
			break;
		}
	}

	if (!p) {
		return -1;
	}

	/* check '"' */
	if (*p == '"') {
		dquotes = true;
		p++;
	}

	while (p) {
		if (dquotes && *p == '"') {
			break;
		}

		/* skip last space **/
		if (isspace((unsigned char)*p) && p[1] == '\0') {
			p++;
			continue;
		}

		if (*p == '\0' || (eopt && *p == *eopt)) {
			break;
		}

		buf[i++] = *p++;
	}

	strcpy(out, buf);

	return 0;
}

/**
 * parse_config - parse config
 * @str : source string
 * @opt : option to search
 * @out : option with 'opt'
 *
 */
static int parse_config(char *str, char *opt, char *out)
{
	char *p;

	if (*str == '#') {
		return -1;
	}

	if (str[0] == '/' && str[1] == '/') {
		return -1;
	}

	p = strstr(str, opt);
	if (!p) {
		return -1;
	}

	strncpy(out, p, strlen(p) - 1);

	return 0;
}

/**
 * get_cmdline_option:
 */
static int get_cmdline_option(char *opt, char *out)
{
	FILE *fp;
	char cmdline[256] = {0, };
	char e = ',';

	fp = fopen("/proc/cmdline", "rb");
	if (!fp)
		return -1;

	if (!fgets(cmdline, sizeof(cmdline), fp)) {
		fclose(fp);
		return -1;
	}

	fclose(fp);
	parse_option(cmdline, opt, out, &e);

	return 0;
}

/**
 * parse_launcher_conf:
 */
static int parse_launcher_conf(const char *file, launcher_t *li, int size)
{
	FILE *fp;
	struct stat st;
	char buf[512] = {0. };
	char *s;
	int i, found = 0;

	fp = fopen(file, "rb");
	if (!fp)
		return 0;

	while (!feof(fp)) {
		s = fgets(buf, sizeof(buf), fp);
		if (!s)
			continue;

		for (i = 0; i < size; i++) {
			if (li[i].found)
				continue;

			if (strstr(s, li[i].s)) {
				if (!parse_config(s, li[i].s,
					li[i].d)) {
					li[i].found = true;
					found++;
					break;
				}
			}
		}
	}

	/*
	 * orignal '/tmp/wayland-runtime-dir'
	 * this tmp is remounted so reposiont to var
	 */
	for (i = 0; i < size; i++) {
		if (li[i].found && li[i].isdir) {
			if (li[i].isdir) {
				char *d = strchr(li[i].d, '/');

				if (d && stat(d, &st)) {
					if (mkdir(d, 0700))
        					error("make dir %s\n", d);
        			}
        		}
    		}
    	}

	fclose(fp);
	return found;
}

#ifdef ENABLE_PRELOAD
/**
 * preload_libs:
 */
static const char *preload_libs[] = {
	/* Empty */
};
#define	LIBS_SIZE ARRAY_SIZE(preload_libs)

/**
 * bootlauncher:
 */
static void *preload_thread(void * arg) {
	char *name = arg;
	int pid = gettid();
	void *dl;

	dl = dlopen(name, RTLD_NOW | RTLD_GLOBAL);
	if (!dl) {
	   	error("[%d] load: %s\n", pid, name);
	}

	return NULL;
}

/**
 * boot_preload:
 */
static int boot_preload(bool load) {
	pthread_t th[LIBS_SIZE];
	char *name;
	int num = 0, i;

	if (!load)
		return 0;

    	for (i = 0; i < LIBS_SIZE; i++) {
		name = (char *)preload_libs[i];
        	if (pthread_create(&th[i], NULL, preload_thread, name)) {
        		error("create thread.%d %s\n", i, strerror(errno));
        	    	break;
        	}
        	num++;
    	}

	for (i = 0 ; i < num; i++) {
		pthread_cancel(th[i]);
		pthread_join(th[i], NULL);
	}

	return 0;
}
#endif

/**
 * bootlauncher:
 */
static int bootlauncher(const char *file, bool debug) {
	char **envp = NULL;
	char prog[256] = {0, }, args[256] = {0, };
#ifdef TSLIB
	char tsdev[256] = {0, }, tscal[256] = {0, }, tsplug[256] = {0, }, pcal[256] = {0, };
#endif
	char lang[256] = {0, };
	char tmp[256] = {0, };
	launcher_t *li = launcher_info;
	int size = ARRAY_SIZE(launcher_info);
	pid_t pid;
	int fd = 0;
	int i, j, n;
#ifdef CHK_BOOTTIME
	long long ts;
	struct timespec tv;
	ts_t ts_s;

	clock_gettime(CLOCK_BOOTTIME, &tv);
	ts = (tv.tv_sec * 1000000) + (tv.tv_nsec / 1000);
	SET_TS(&ts_s, ts);
	RUN_TS(&ts_s);
#endif

	message("[%d] bootlauncher\n", getpid());

	n = parse_launcher_conf(file, li, size);
	if (!n) {
		error("not exist config: %s\n", file);
		return EXIT_SUCCESS;
	}

	/* parse environments */
	envp = malloc(sizeof(char*) * n + 1);
	for (i = 0, j = 0; i < size; i++) {
		if (li[i].found) {
			envp[j++] = li[i].d;
			if (debug) {
				message("PARSE:%s\n", li[i].d);
			}
		}
	}
	envp[j] = NULL;

	/* parse launcher program, args and ts */
	for (i = 0; i < size; i++) {
		if (!li[i].found) {
			continue;
		}

		if (!parse_option(li[i].d, RUN_LAUNCHER, tmp, NULL)) {
			strcpy(prog, tmp);
			continue;
		}

		if (!parse_option(li[i].d, RUN_LAUNCHER_ARGS, tmp, NULL)) {
			strcpy(args, tmp);
			continue;
		}

#ifdef TSLIB
		if (!parse_option(li[i].d, SET_TSDEVICE, tmp, NULL)) {
			strcpy(tsdev, tmp);
			continue;
		}

		if (!parse_option(li[i].d, SET_TSCAL, tmp, NULL)) {
			strcpy(tscal, tmp);
			continue;
		}

		if (!parse_option(li[i].d, SET_TSPLUG, tmp, NULL)) {
			strcpy(tsplug, tmp);
			continue;
		}

		if (!parse_option(li[i].d, SET_PCAL, tmp, NULL)) {
			strcpy(pcal, tmp);
			continue;
		}
#endif
		if (!parse_option(li[i].d, SET_LANG, tmp, NULL)) {
			strcpy(lang, tmp);
			continue;
		}
	}

	if (debug) {
		message("ENV:\n");
		for (i = 0; i < j; i++) {
			message("%s\n", envp[i]);
		}
		message("EXE: %s %s\n", prog, args);
	}

#ifdef ENABLE_PRELOAD
	/*
	 * preload libraries for launcher
	 */
	boot_preload(false);
#endif

#ifdef ENABLE_CONSOLE
	struct sigaction sigchld_action;
	sigchld_action.sa_handler = SIG_DFL;
	sigchld_action.sa_flags = SA_NOCLDWAIT;
	sigaction(SIGCHLD, &sigchld_action, NULL);
#endif

	if (strlen(prog)) {
		pid = fork();
		/* run lanucher */
		if (pid == 0) {
			if (!debug) {
#ifdef ENABLE_CONSOLE
  				fd = open("/dev/console", O_RDWR);
#else
				fd = open("/dev/null", O_RDWR);
#endif
				dup2(fd, STDOUT_FILENO);
				dup2(fd, STDERR_FILENO);
			}

#ifdef TSLIB
			/* set TS */
			setenv(SET_TSDEVICE, tsdev, 1);
			setenv(SET_TSCAL, tscal, 1);
			setenv(SET_TSPLUG, tsplug, 1);
			setenv(SET_PCAL, pcal, 1);
#endif
			setenv(SET_LANG, lang, 1);

			/* execute launcher */
			execle(prog, prog, args, NULL, envp);

			if (fd > 0) {
				close(fd);
			}
		}

		/*
	 	 * wait for launcher run
	 	 * During this time, the system will be hold.
	 	*/
		usleep(BOOTTIME_LAUNCHER);
	}

#ifdef CHK_BOOTTIME
  	END_TS(&ts_s);
  	message("qtbootlauncher is finished : %lld ms\n", TS_MS(&ts_s));
#endif

	return EXIT_SUCCESS;
}

static void print_usage(void) {
	printf("\n");
	printf("usage: options\n");
	printf("-f parse file (%s)\n", LAUNCHER_CONF);
	printf("-d debug\n");
}

int main(int argc, char *argv[]) {
	int opt;
	const char *file = LAUNCHER_CONF;
	bool debug = false;

	while (-1 != (opt = getopt(argc, argv, "hf:d"))) {
		switch (opt) {
		case 'h':
			print_usage();
			exit(0);
			break;
		case 'f':
			file = optarg;
			break;
		case 'd':
			debug = true;
			break;
		default:
			print_usage();
			exit(0);
		};
	}
	get_cmdline_option("console", console);

	return bootlauncher(file, debug);
}
