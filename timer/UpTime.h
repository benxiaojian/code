// read linux running time 

#ifndef __UpTime_h__
#define __UpTime_h__

#include <time.h>
#include <fcntl.h>
//#include <timer/Timer.h>
#include <kpiCollector/base.h>
#include <system/Thread.h>

typedef u64 uptime_t;
static const char m_fileName[] = "/proc/uptime";

typedef struct _UpTime{
	Mutex* m_uptimeFpReadMutex;
	FILE* m_uptimeFp;
	int singleThreaded;
}UpTime;

uptime_t GetMillisecs(UpTime* m_uptime);
void ReadUpTimeFile(UpTime* m_uptime, unsigned int* seconds, unsigned int* hundredths);
void SetCloExec(int fd);

static void _uptime_t_init(const void* cpv_input, void* pv_output)
{
	*(uptime_t*)cpv_input = 0;
	*(bool_t*)pv_output = true;
}

static void _uptime_t_destroy(const void* cpv_input, void* pv_output)
{
	*(uptime_t*)cpv_input = 0;
	*(bool_t*)pv_output = true;
}

static void _uptime_t_copy(const void* cpv_first, const void* cpv_second, void* pv_output)
{
	*(uptime_t*)cpv_first = *(uptime_t*)cpv_second;
	*(bool_t*)pv_output = true;
}

static void _uptime_t_less(const void* cpv_first, const void* cpv_second, void* pv_output)
{
	*(bool_t*)pv_output = *(uptime_t*)cpv_first < *(uptime_t*)cpv_second ? true : false;
}

UpTime* UpTimeCreate(Bool singleThreaded);
void UpTimeDestroy(UpTime** m_uptime);
uptime_t GetMillisecs(UpTime* m_uptime);
void ReadUpTimeFile(UpTime* m_uptime, unsigned int* seconds, unsigned int* hundredths);
void SetCloExec(int fd);

#endif