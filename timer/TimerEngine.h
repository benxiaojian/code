#ifndef __TimerEngine_h__
#define __TimerEngine_h__

#include "Timer.h"
#include "UpTime.h"
#include "TimerInterface.h"
#include <time.h>
#include <sys/time.h>
#include <system/Thread.h>
#include <cstl/cmap.h>
#include <cstl/clist.h>


static const u32 MAX_TIMERS = 0xffff;
// map<TimerHandle, Timer*> TimerMap;
// ????? change to <TimerHandle, Timer>???, Timer* may be used failed
typedef map_t* TimerMap;

// multimap <uptime_t, TimerHandle> TimerExpiryMap;
// change to map<TimerHandle, uptime_t> TimerExpiryMap
typedef map_t* TimerExpiryMap;

// list <Timer> TimerList;
typedef list_t* TimerList;

typedef struct _TimerEngine{
	Mutex* m_timersMutex;
	Condition* m_timersCondition;
	UpTime* m_uptime;

	TimerMap m_theTimers;
	TimerExpiryMap m_runningTimers;
	list_t* m_expiredTimers;
}TimerEngine;

typedef struct _TimerEngineThread{
	Thread* thread;
	Mutex  m_timersMutex;
    Condition      m_timersCondition;
}TimerEngineThread;

TimerEngineThread* m_timerEngineThread;
TimerEngine* m_timerEngine;

TimerEngine* GetTimeEngineInstance();

TimerEngineThread* GetTimeEngineThreadInstance();

TimerEngine* TimerEngineCreate(Mutex* mutex, Condition* condition);

void TimerEngineDestroy();

TimerHandle CreateTimer(const char* timerName,
						u32 timeoutPeriodMs,
						void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
						TimerStart startNow,
						TimerMode timerMode,
						Bool traceTimerEvents);

TimerHandle CreateWallClockTimer(const char* timerName,
								 u32 timeoutPeriodMinutes,
								 void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
								 TimerStart startNow,
								 TimerMode timerMode,
							     Bool traceTimerEvents);

static TimerHandle CreateAnyTimer(	const char* timerName,
						   	u32 timeoutPeriodMs,
							void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
							TimerStart startNow,
							TimerMode timerMode,
							TimerAlignment timerAlignment,
							Bool traceTimerEvents);

void StartTimer(TimerHandle m_timerHandle);

void StopTimer(TimerHandle m_timerHandle);

    /**
     * Calculate timeout period for a wall clock aligned timer.
     *
     * E.g. if the time is 13:42, and a one hour timeout is requested, the timer will
     * first expire in 18 minutes at 14:00.  If a 5 minute timer is requested, the first
     * expiry will be at 13:45.
     *
     * \param timerPeriodMillisecs  Nominal timer period, 0->24hrs
     * \return                      Millisecs to next timer expiry.
     */
u32 CalculateWallClockExpiryTime(u32 timerPeriodMillisecs);

u32 GetMillisecsToNextTimerExpiry();

Timer* FindTimer(map_t* map, TimerHandle* key);

TimerEngineThread* TimerEngineThreadCreate();

void TimerEngineThreadDestroy();

u32 TimerEngineThreadProcedure();

void StartTimerEngineThread();

uptime_t GetTickCountMs();

void CheckTimer();

void DeleteOrRestartExpiredTimers();

void DeleteTimer(TimerHandle timerHandle);

#endif 