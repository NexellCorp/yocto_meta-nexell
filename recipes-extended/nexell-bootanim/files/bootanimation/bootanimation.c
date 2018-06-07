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
#include <linux/sched.h>
//#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>

#include "fb.h"
#include "bmp.h"
#include "jz.h"
#include "png.h"

#define	FB_DEV			"/dev/fb0"
#define	FPS_HZ			30
#define	DBG_MS			5
#define	FB_TRY_MS		1000
#define	ANIMATION_CONF		"/etc/qboot/animation.conf"

/*
 * for: /etc/qboot/animation.conf
 */
#define	FRAMEBUFFER_DEV		"FRAMEBUFFER"
#define	STIL_IMAGE_PATH		"STIL_IMAGE"
#define	STIL_IMAGE_TIME		"STIL_TIME"
#define	ANIMATION_PATH		"ANIMATION_IMAGE"
#define	ANIMATION_FPS		"ANIMATION_FPS"
#define	ANIMATION_TIME		"ANIMATION_TIME"
#define	BOOT_EXE			"BOOT_EXE"

#define	FPS_MS(f)		(1000/f)

/**
 * ARRAY_SIZE:
 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

/**
 * TIME STAMP:
 */
#define	RUN_TIMESTAMP_US(s) {		\
		struct timespec tv;	\
		clock_gettime(CLOCK_BOOTTIME, &tv); \
		s = (tv.tv_sec*1000000) + (tv.tv_nsec/1000);	\
	}

#define	END_TIMESTAMP_US(s, d)	{	\
		struct timespec tv;	\
		clock_gettime(CLOCK_BOOTTIME, &tv); \
		d = (tv.tv_sec*1000000) + (tv.tv_nsec/1000);	\
		d = d - s;	\
	}

typedef struct {
	long long ts;
	long long td;
} ts_t;

#define	SET_TS(s, t)	(((ts_t *)s)->ts = t)
#define	RUN_TS(s)	RUN_TIMESTAMP_US(((ts_t *)s)->ts)
#define	END_TS(s)	END_TIMESTAMP_US(((ts_t *)s)->ts, ((ts_t *)s)->td)
#define	TS_US(s)	(((ts_t *)s)->ts)
#define	TS_MS(s)	(((ts_t *)s)->td/1000)

/**
 * bootanimation_t:
 */
typedef struct {
	char framebuffer[32];
	char stil_img[256];
	unsigned int stil_time_ms;
	char anim_img[256];
	int anim_fps;
	unsigned int anim_time_ms;
	char exec[256];
} bootanimation_t;

/**
 * message:
 */
#define	__msg__(m...)		printf(m)
static char console_device[64];
static fb_t *pFB;

static void message(const char *f, ...)
{
	FILE *fd;
	va_list args;
	char buf[512] = { 0, };
	char tty[512] = { 0, };

	sprintf(tty, "/dev/%s", console_device);
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

/**
 * parse_option - parse option
 * @str : source string
 * @opt : option to search
 * @out : option
 * @eopt : option to line end
 */
static int parse_option(char *str, char *opt, char *out, char *eopt)
{
	char *p, *e;
	char buf[256] = { 0, };
	int i = 0;
 	bool dquotes = false;	/* " */

	if (*str == '#')
		return -1;

	if (str[0] == '/' && str[1] == '/')
		return -1;

	/* search opt with str */
	p = strstr(str, opt);
	if (!p)
		return -1;

	/* check total opt in case opt:AAA, str:AAA_BBB */
	e = p + strlen(opt);
	if (!isspace((unsigned char)*e) && *e != '=')
		return -1;

	/* go '=' */
	p = strstr(e, "=");
	if (!p)
		return -1;

	/* go first char */
	while (p++) {
		if (!isspace((unsigned char)*p))
			break;
	}

	if (!p)
		return -1;

	/* check '"' */
	if (*p == '"') {
		dquotes = true;
		p++;
	}

	while (p) {
		if (dquotes && *p == '"')
			break;

		/* skip last space **/
		if (isspace((unsigned char)*p) && p[1] == '\0') {
			p++;
			continue;
		}

		if (*p == '\0' || (eopt && *p == *eopt))
			break;

		buf[i++] = *p++;
	}

	strcpy(out, buf);

	return 0;
}

/**
 * get_cmdline_option:
 */
static int get_cmdline_option(char *opt, char *out)
{
	FILE *fp;
	char cmdline[256];
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
 * parse_animation_conf:
 */
static int parse_animation_conf(const char *file, bootanimation_t *ani)
{
	FILE *fp;
	char buf[256] = { 0, };
	char tmp[16] = { 0, };

	fp = fopen(file, "rb");
	if (!fp)
		return -1;

	while (!feof(fp)) {
		char *s = fgets(buf, sizeof(buf), fp);
		if (!s)
			continue;

		/*
		 * parset framebuffer config
		 */
		if (strstr(s, FRAMEBUFFER_DEV)) {
			parse_option(s, FRAMEBUFFER_DEV, ani->framebuffer, NULL);
			continue;
		}

		/*
		 * parset stil image config
		 */
		if (strstr(s, STIL_IMAGE_PATH)) {
			parse_option(s, STIL_IMAGE_PATH, ani->stil_img, NULL);
			continue;
		}

		if (strstr(s, STIL_IMAGE_TIME)) {
			if (!parse_option(s, STIL_IMAGE_TIME, tmp, NULL))
				ani->stil_time_ms = strtol(tmp, NULL, 10);
			continue;
		}

		/*
		 * parset animation config
		 */
		if (strstr(s, ANIMATION_PATH)) {
			parse_option(s, ANIMATION_PATH, ani->anim_img, NULL);
			continue;
		}

		if (strstr(s, ANIMATION_FPS)) {
			if (!parse_option(s, ANIMATION_FPS, tmp, NULL)) {
				printf("option : %s\n", tmp);
				ani->anim_fps = strtol(tmp, NULL, 10);
				printf("fps = %d\n", ani->anim_fps);
			}
			continue;
		}

		if (strstr(s, ANIMATION_TIME)) {
			if (!parse_option(s, ANIMATION_TIME, tmp, NULL))
				ani->anim_time_ms = strtol(tmp, NULL, 10);
			continue;
		}

		/* parset boot exe */
		if (strstr(s, BOOT_EXE)) {
			parse_option(s, BOOT_EXE, ani->exec, NULL);
			continue;
		}
	}

	fclose(fp);
	return 0;
}

/**
 * set_sched_new:
 */
static int set_sched_new(pid_t pid, int policy, int priority)
{
	struct sched_param param;
	int maxpri = 0, minpri = 0;
	int ret;

	switch(policy) {
	case SCHED_NORMAL:
	case SCHED_FIFO:
	case SCHED_RR:
	case SCHED_BATCH:
		break;
	default:
		error("Invalid policy %d (0~3)\n", policy);
		return -1;
	}

	if(SCHED_NORMAL == policy) {
		/*
		 * #define NICE_TO_PRIO(nice)
		 * (MAX_RT_PRIO + (nice) + 20), MAX_RT_PRIO 100
		 */
		maxpri =  20;
		/* nice */
		minpri = -20;
	} else {
		maxpri = sched_get_priority_max(policy);
		minpri = sched_get_priority_min(policy);
	}

	if((priority > maxpri) || (minpri > priority)) {
		error("Invalid priority (%d ~ %d)...\n",
			minpri, maxpri);
		return -1;
	}

	if(SCHED_NORMAL == policy) {
		param.sched_priority = 0;
		ret = sched_setscheduler(pid, policy, &param);
		if(ret) {
			error("sched_setscheduler(ret %d), %s\n",
				ret, strerror(errno));
			return ret;
		}
		ret = setpriority(PRIO_PROCESS, pid, priority);
		if(ret)
			error("setpriority(ret %d), %s\n\n",
				ret, strerror(errno));
	} else {
		param.sched_priority = priority;
		ret = sched_setscheduler(pid, policy, &param);
		if(ret)
			error("sched_setscheduler(ret %d), %s\n\n",
				ret, strerror(errno));
	}

	return ret;
}

/**
 * signal_handler:
 */
static void signal_handler(int sig)
{
	fb_t *fb = pFB;
	message("\n Signal.%d %s\n",
		sig, (char *)strsignal(sig));

	fb_flip(fb, 0);
	exit(0);
}

/**
 * signal_register:
 */
static void signal_register(void)
{
	struct sigaction sact;

	sact.sa_handler = signal_handler;
  	sigemptyset(&sact.sa_mask);
  	sact.sa_flags = 0;

	sigaction(SIGTERM, &sact, 0);
}

/**
 * bootanimation:
 */
static int bootanimation(const char *fbname,
			const char *stilname, unsigned int stiltime_ms,
			const char *zipname, unsigned int anitime_ms, int fps)
{
	fb_t *fb;
	bmp_t *bmp = NULL;
	zip_t *zip = NULL;
	png_t **pngs = NULL, *png = NULL;

	int z = 0, n = 0, m = 0;
	int i, pan = 0;

	long long ts, td;
	struct timespec tv;
	ts_t ts_s, ts_png, ts_pix, ts_fps;
	int count = 0;

	if (!fbname)
		return 0;

	message("[%d] bootanimation to '%s'\n", (int)getpid(), fbname);
	message("stil: '%s', %u ms\n", stilname, stiltime_ms);
	message("ani : '%s', %u ms %dHZ\n", zipname, anitime_ms, fps);

	signal_register();
	set_sched_new(getpid(), SCHED_FIFO, 99);

	/*
	 * Load framebuffer Try loop
	 */
	RUN_TS(&ts_s);
	for (i = 0; i < FB_TRY_MS; i++) {
		fb = fb_initialize(fbname);
		if (fb)
			break;
		usleep(1000 * 1000);
	}
	END_TS(&ts_s);
	message("%s Load framebuffer [%s] %lldms <%d>\n",
		fb ? "[  OK  ]" : "[FAILED]", fbname, TS_MS(&ts_s), i);

	if (!fb)
		return 0;

	pFB = fb;

	/*
	 * Load stil image with BMP
	 */
	if (stilname) {
		RUN_TS(&ts_s);
		bmp = bmp_load(stilname, NULL);
		END_TS(&ts_s);
		message("%s Load bmp %lldms\n",
			bmp ? "[  OK  ]" : "[FAILED]", TS_MS(&ts_s));

		/* Draw stil image duratioin stiltime_ms  */
		if (bmp) {
			fb_bmp_draw(fb, bmp, 0);
			fb_flip(fb, 0);
			usleep(stiltime_ms * 1000);
		}
	}

	/*
	 * decompress bootanimation with ZIP
	 */
	if (zipname) {
		RUN_TS(&ts_s);
		zip = jzip_decompress(NULL, zipname);
		END_TS(&ts_s);
		message("%s Load and unzip %lldms\n",
			zip ? "[  OK  ]" : "[FAILED]", TS_MS(&ts_s));
	}

	if (!bmp && !zip) {
		message("[FAILED] Not found stil and bootanimation !\n");
		goto out;
	}

	pngs = malloc(sizeof(png_t *) * zip->entries);
	if (!pngs) {
		error("No png entries from zip - %d", zip->entries);
		goto out;
	}

	/*
	 * bootanimation
	 */
	while (1) {
		clock_gettime(CLOCK_BOOTTIME, &tv);
		ts = (tv.tv_sec * 1000000) + (tv.tv_nsec/1000);

		SET_TS(&ts_png, ts);
		SET_TS(&ts_fps, ts);

		/* Load png */
		if (z < zip->entries) {
			pngs[n] = png_load(NULL, zip->decomps[z++]);
			if (!pngs[n])
				continue;
			png = pngs[n];
			n++, m = n;
		} else {
			if (m == 0) {
				error("no exist pngs ...\n");
				exit(0);
			}
			n++, n %= m;
			png = pngs[n];
		}
		END_TS(&ts_png);

		/*
		 * rendering to pan display
		 */
		pan++, pan %= fb->pans;

		RUN_TS(&ts_pix);
		fb_png_draw(fb, png, pan);
		END_TS(&ts_pix);

		clock_gettime(CLOCK_BOOTTIME, &tv);
		td = (((tv.tv_sec * 1000000) + (tv.tv_nsec/1000)) - ts)/1000;

		if (FPS_MS(fps) > td) {
			long w = FPS_MS(fps) - td;
			fb_flip(fb, pan);
			usleep(w * 1000);
		}

		END_TS(&ts_fps);
		count++;

		if (0 == (count % (FPS_HZ * DBG_MS)))
			message("[%d:%3d] - %lld - %lld - all %lld ms [%d:%d]\n",
				pan, n, TS_MS(&ts_png),
				TS_MS(&ts_pix), TS_MS(&ts_fps), zip->entries, m);

		if (count * FPS_MS(fps) > anitime_ms)
			break;
	}

out:
	if (pngs) {
		for (i = 0; i < m; i++)
			png_release(pngs[i]);
	}

	fb_release(fb);
	jzip_release(zip);
	bmp_release(bmp);

	return 0;
}

static void print_usage(void)
{
	printf("\n");
	printf("usage: options\n");
	printf("-d framebuffer device path (/dev/fb0)\n");
	printf("-s stilimage path\n");
	printf("-t stilimage ms time\n");
	printf("-z animiation zip path\n");
	printf("-f animiation fps (%d)\n", FPS_HZ);
	printf("-a animiation ms time\n");
}

int main(int argc, char *argv[])
{
	int opt;
	const char *file = ANIMATION_CONF;
	pid_t pid;

	bootanimation_t ani_t = {
		.framebuffer = FB_DEV,
		.stil_img = { 0, },
		.stil_time_ms = 1,
		.anim_img = { 0, },
		.anim_fps = 30,
		.anim_time_ms = -1,
		.exec = "/sbin/init",
	};

	parse_animation_conf(file, &ani_t);

	while (-1 != (opt = getopt(argc, argv, "hd:s:t:z:f:a:"))) {
		switch (opt) {
		case 'h':
			print_usage();
			exit(0);
			break;
		case 'd':
			strcpy(ani_t.framebuffer, optarg);
			break;
		case 's':
			strcpy(ani_t.stil_img, optarg);
			break;
		case 't':
			ani_t.stil_time_ms = strtol(optarg, NULL, 10);
			break;
		case 'z':
			strcpy(ani_t.anim_img, optarg);
			break;
		case 'f':
			ani_t.anim_fps = strtol(optarg, NULL, 10);
			break;
		case 'a':
			ani_t.anim_time_ms = strtol(optarg, NULL, 10);
			break;
		default:
			print_usage();
			exit(0);
			break;
		};
	}

	/* for debug with /proc/cmdline */
	get_cmdline_option("console", console_device);
	message("[%s]\n", argv[0]);

	if ((int)getpid() == 1) {
		char *init = ani_t.exec;
		char *arg = (strrchr(ani_t.exec, '/') + 1);

		pid = fork();

		/* child: run bootanimation */
		if (pid == 0)
			return bootanimation(ani_t.framebuffer,
				ani_t.stil_img, ani_t.stil_time_ms,
				ani_t.anim_img, ani_t.anim_time_ms, ani_t.anim_fps);

		execl(init, arg, NULL);
	}

	return bootanimation(ani_t.framebuffer,
				ani_t.stil_img, ani_t.stil_time_ms,
				ani_t.anim_img, ani_t.anim_time_ms, ani_t.anim_fps);
}
