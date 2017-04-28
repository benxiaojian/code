#ifndef __ThreadlessTimerEngine_h__
#define __ThreadlessTimerEngine_h__

#include "TimerInterface.h"
#include "Timer.h"
#include "UpTime.h"
#include "TimerEngine.h"
#include <math.h>
#include <system/Selector.h>
#include <cstl/cmap.h>
#include <cstl/clist.h>
#include <time.h>
#include <sys/time.h>
#include <Radisys.h>

// static const u32 MAX_TIMERS = 0xffff;

// //map<TimerHandle, Timers> TimerMap
// typedef struct map_t* TimerMap;

// // multimap <uptime_t, TimerHandle> TimerExpiryMap;
// // change to map<TimerHandle, uptime_t> TimerExpiryMap
// typedef map_t* TimerExpiryMap;

// //list<Timer> TimerList;
// typedef list_t* TimerList;

typedef struct _ThreadlessTimerEngine{
	UpTime* m_uptime;
	TimerMap m_theTimers;
	TimerExpiryMap m_runningTimers;
	list_t* m_expiredTimers;

	Selector* m_selector;	
}ThreadlessTimerEngine;

ThreadlessTimerEngine* ThreadlessTimerEngineCreate(Selector* m_selector);

void ThreadlessTimerEngineDestroy(ThreadlessTimerEngine** threadlessTimerEngine);

TimerHandle CreateTimerWithoutThread(ThreadlessTimerEngine* m_threadlessTimerEngine,
									 const char* timerName,
									 u32 timeoutPeriodMs,
							  		 void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
									 TimerStart startNow,
									 TimerMode timerMode,
									 Bool traceTimerEvents);

TimerHandle CreateWallClockTimerWithoutThread(ThreadlessTimerEngine* m_threadlessTimerEngine,
										      const char* timerName,
									 		  u32 timeoutPeriodMinutes,
							  		 		  void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
									 		  TimerStart startNow,
									 		  TimerMode timerMode,
									 		  Bool traceTimerEvents);

TimerHandle CreateAnyTimerWithoutThread(ThreadlessTimerEngine* m_threadlessTimerEngine,
										const char* timerName,
										u32 timeoutPeriodMs,
										void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
										TimerStart startNow,
										TimerMode timerMode,
										TimerAlignment timerAlignment,
										Bool traceTimerEvents);

void ThreadlessStartTimer(ThreadlessTimerEngine* m_threadlessTimerEngine, TimerHandle timerHandle);
void ThreadlessStopTimer(ThreadlessTimerEngine* m_threadlessTimerEngine, TimerHandle timerHandle);

u32 ThreadlessCalculateWallClockExpiryTime(u32 timerPeriodMillisecs);

void ThreadlessSelectorCallBack(void* args);

void ThreadlessCheckTimers(ThreadlessTimerEngine* m_threadlessTimerEngine);

void ThreadlessDeleteOrRestartExpiryTimers(ThreadlessTimerEngine* m_threadlessTimerEngine);

void ThreadlessDeleteTimer(ThreadlessTimerEngine* m_threadlessTimerEngine, TimerHandle timerHandle);

void ThreadlessCheckHeadTimer(ThreadlessTimerEngine* m_threadlessTimerEngine);



#endif