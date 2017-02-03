#ifndef _UTIL_DEBUG_H_
#define _UTIL_DEBUG_H_

/*
#define DEBUG
*/

#define	pr_info(m...)		printf(m)
#define	pr_err(m...)		printf(m)

#ifdef DEBUG
#define	pr_debug(m...)
#define	pr_aplay(m...)		printf(m)
#define	pr_stream(m...)		printf(m)
#define	pr_main(m...)		printf(m)
#else
#define	pr_debug(m...)
#define	pr_debug(m...)
#define	pr_aplay(m...)
#define	pr_stream(m...)
#define	pr_main(m...)
#endif

#endif /* _UTIL_DEBUG_H_ */