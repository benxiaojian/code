#ifndef __Timer_h__
#define __Timer_h__

#include <kpiCollector/base.h>
#include "TimerInterface.h"
TimerHandle s_nextTimerHandle; // Seeding used when creating new timers. 
								// distribution timer id
typedef struct _Timer{
	TimerHandle m_timerHandle; // timer id
	u32 m_timerPeriodMs; //  time length
	TimerMode m_timerMode; // timer end mode
	char* m_timerName; //timer name
	// callback func, after timer end
	void (*NotifyTimerExpiry)(TimerHandle expiredTimer); 
	TimerAlignment m_timerAlignment; // timer type
	Bool m_traceTimerEvents; // trace timer ?
}Timer;

Timer* TimerCreate(u32 timeoutPeriodMs, 
				   TimerMode timerMode, 
				   const char* timerName,
				   void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
				   TimerAlignment timerAlignment,
				   bool traceTimerEvents);

void TimerDestroy(Timer** timer);

static void _Timer_init(const void* cpv_input, void* pv_output)
{
	((Timer*)cpv_input)->m_timerHandle = NULL_TIMER;
	((Timer*)cpv_input)->m_timerPeriodMs = 0;
	((Timer*)cpv_input)->m_timerMode = TIMER_SINGLE_SHOT;
	((Timer*)cpv_input)->m_timerName = NULL;
	((Timer*)cpv_input)->NotifyTimerExpiry = NULL;
	((Timer*)cpv_input)->m_timerAlignment = TIMER_UNALOGNED;
	((Timer*)cpv_input)->m_traceTimerEvents = 0;
	*(bool_t*)pv_output = true;
}

static void _Timer_destroy(const void* cpv_input, void* pv_output)
{
	((Timer*)cpv_input)->m_timerHandle = NULL_TIMER;
	((Timer*)cpv_input)->m_timerPeriodMs = 0;
	((Timer*)cpv_input)->m_timerMode = TIMER_SINGLE_SHOT;
	free(((Timer*)cpv_input)->m_timerName);
	((Timer*)cpv_input)->m_timerName = NULL;
	((Timer*)cpv_input)->NotifyTimerExpiry = NULL;
	((Timer*)cpv_input)->m_timerAlignment = TIMER_UNALOGNED;
	((Timer*)cpv_input)->m_traceTimerEvents = 0;
	*(bool_t*)pv_output = true;
}

static void _Timer_copy(const void* cpv_first, const void* cpv_second, void* pv_output)
{
	((Timer*)cpv_first)->m_timerHandle = ((Timer*)cpv_second)->m_timerHandle;
	((Timer*)cpv_first)->m_timerPeriodMs = ((Timer*)cpv_second)->m_timerPeriodMs;
	((Timer*)cpv_first)->m_timerMode = ((Timer*)cpv_second)->m_timerMode;

	//((Timer*)cpv_first)->m_timerName = (char*)malloc(strlen(((Timer*)cpv_second)->m_timerName)*sizeof(char));
	//strcpy(((Timer*)cpv_first)->m_timerName,((Timer*)cpv_second)->m_timerName);
	((Timer*)cpv_first)->m_timerName = ((Timer*)cpv_second)->m_timerName;
	((Timer*)cpv_first)->NotifyTimerExpiry = ((Timer*)cpv_second)->NotifyTimerExpiry;
	((Timer*)cpv_first)->m_timerAlignment = ((Timer*)cpv_second)->m_timerAlignment;
	((Timer*)cpv_first)->m_traceTimerEvents = ((Timer*)cpv_second)->m_traceTimerEvents;
	*(bool_t*)pv_output = true;
}


static void _Timer_less(const void* cpv_first, const void* cpv_second, void* pv_output)
{
	*(bool_t*)pv_output = ((Timer*)cpv_first)->m_timerHandle < ((Timer*)cpv_second)->m_timerHandle ? true : false;
}

#endif