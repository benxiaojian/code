#include "Timer.h"

Timer* TimerCreate(u32 timeoutPeriodMs, 
				   TimerMode timerMode, 
				   const char* timerName,
				   void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
				   TimerAlignment timerAlignment,
				   bool traceTimerEvents)
{
	ENTRY;
	Timer* m_timer = (Timer*)malloc(sizeof(Timer));
	if (!m_timer)
	{
		#ifdef DEBUG
		printf("timer malloc failed\n");
		#endif

		return NULL;
	}

	if (++s_nextTimerHandle == NULL_TIMER)
	{
		++s_nextTimerHandle;
	}
	m_timer->m_timerHandle = s_nextTimerHandle;

	m_timer->m_timerPeriodMs = timeoutPeriodMs;
	m_timer->m_timerMode = timerMode;
	m_timer->m_timerName = (char*)malloc(strlen(timerName)*sizeof(char));
	strcpy(m_timer->m_timerName, timerName);
	m_timer->NotifyTimerExpiry = NotifyTimerExpiry;
	m_timer->m_timerAlignment = timerAlignment;
	m_timer->m_traceTimerEvents = traceTimerEvents;

	return m_timer;
}

void TimerDestroy(Timer** timer)
{
	(*timer)->m_timerName = NULL;
	(*timer)->NotifyTimerExpiry = NULL;
	free(*timer);
}