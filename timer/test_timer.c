#include "Timer.h"
#include "UpTime.h"
#include "TimerInterface.h"
#include "TimerEngine.h"
#include "ThreadlessTimerEngine.h"

//#define TIMER_WITH_THREAD
#define TIMER_WITHOUT_THREAD

void NotifyTimerExpiry(TimerHandle expiredTimer)
{
	ENTRY;
	printf("timer %d expried to do something\n",expiredTimer);
}

int main()
{
	#ifdef TIMER_WITH_THREAD
	
	TimerEngineCreate(NULL,NULL);
	TimerEngineThreadCreate();
	StartTimerEngineThread();
	CreateTimer("timer1", 1000, NotifyTimerExpiry,TIMER_START_NOW,TIMER_SINGLE_SHOT_WITHOUT_DELETE,0);
	CreateTimer("timer2", 6000, NotifyTimerExpiry,TIMER_START_NOW,TIMER_SINGLE_SHOT_WITHOUT_DELETE,0);
	CreateTimer("timer3", 2000, NotifyTimerExpiry,TIMER_START_NOW,TIMER_SINGLE_SHOT_WITHOUT_DELETE,0);
	CreateTimer("timer4", 6000, NotifyTimerExpiry,TIMER_START_NOW,TIMER_SINGLE_SHOT_WITHOUT_DELETE,0);
	// CreateWallClockTimer("timer1", 60, NotifyTimerExpiry,TIMER_START_NOW, TIMER_SINGLE_SHOT_WITHOUT_DELETE,0);

	#endif

	#ifdef TIMER_WITHOUT_THREAD

	Selector* selector = SelectorCreate();
	ThreadlessTimerEngine* threadlessTimerEngine = ThreadlessTimerEngineCreate(selector);
	CreateTimerWithoutThread(threadlessTimerEngine,"timer1", 1000, NotifyTimerExpiry,TIMER_START_NOW,TIMER_SINGLE_SHOT_WITHOUT_DELETE,0);
	CreateTimerWithoutThread(threadlessTimerEngine,"timer3", 2000, NotifyTimerExpiry,TIMER_START_NOW,TIMER_SINGLE_SHOT_WITHOUT_DELETE,0);

	while (1)
	{
		printf("DoSelect\n");
		DoSelect(selector,1000);
	}
	#endif
	sleep(100000);
}