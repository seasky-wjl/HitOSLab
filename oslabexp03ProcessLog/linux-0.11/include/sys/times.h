#ifndef _TIMES_H
#define _TIMES_H

#include <sys/types.h>

struct tms {
	time_t tms_utime; // User CPU time. 用户态时间
	time_t tms_stime; // System CPU time. 内核态时间
	time_t tms_cutime; // User CPU time of dead children. 已死掉进程的用户态CPU时间
	time_t tms_cstime; // System CPU time of dead children . 已死掉子进程消耗的内核态时间
};

extern time_t times(struct tms * tp);

#endif
