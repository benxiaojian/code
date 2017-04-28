#include "ThreadlessTimerEngine.h"


ThreadlessTimerEngine* ThreadlessTimerEngineCreate(Selector* m_selector)
{
	ENTRY;
	ThreadlessTimerEngine* threadlessTimerEngine;
	threadlessTimerEngine = (ThreadlessTimerEngine*)malloc(sizeof(ThreadlessTimerEngine));
	if (!threadlessTimerEngine) 
	{
		#ifdef DEBUG
		printf("threadlessTimerEngine malloc failed\n");
		#endif

		return NULL;
	}

	threadlessTimerEngine->m_uptime = m_selector->m_uptime;

	type_register(TimerHandle, _TimerHandle_init, _TimerHandle_copy, _TimerHandle_less, _TimerHandle_destroy);
	type_register(uptime_t, _uptime_t_init, _uptime_t_copy, _uptime_t_less, _uptime_t_destroy);
	type_register(Timer, _Timer_init, _Timer_copy, _Timer_less, _Timer_destroy);

	threadlessTimerEngine->m_theTimers = create_map(TimerHandle,Timer);
	threadlessTimerEngine->m_runningTimers = create_map(TimerHandle,uptime_t);
	threadlessTimerEngine->m_expiredTimers = create_list(Timer);

	map_init(threadlessTimerEngine->m_theTimers);
	map_init(threadlessTimerEngine->m_runningTimers);
	list_init(threadlessTimerEngine->m_expiredTimers);

	threadlessTimerEngine->m_selector = m_selector;


	return threadlessTimerEngine;
}

void ThreadlessTimerEngineDestroy(ThreadlessTimerEngine** threadlessTimerEngine)
{
	(*threadlessTimerEngine)->m_uptime = NULL;
	(*threadlessTimerEngine)->m_selector = NULL;
	map_destroy((*threadlessTimerEngine)->m_theTimers);
	map_destroy((*threadlessTimerEngine)->m_runningTimers);
	list_destroy((*threadlessTimerEngine)->m_expiredTimers);

	free((*threadlessTimerEngine));
	(*threadlessTimerEngine) = NULL;
}

TimerHandle CreateTimerWithoutThread(ThreadlessTimerEngine* m_threadlessTimerEngine,
									 const char* timerName,
									 u32 timeoutPeriodMs,
							  		 void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
									 TimerStart startNow,
									 TimerMode timerMode,
									 Bool traceTimerEvents)
{
	ENTRY;

	TimerHandle timerHandle = CreateAnyTimerWithoutThread(m_threadlessTimerEngine,
		 												  timerName,
		 												  timeoutPeriodMs,
		 												  NotifyTimerExpiry,
		 												  startNow,
		 												  timerMode,
		 												  TIMER_UNALOGNED,
		 												  traceTimerEvents);
	Exit;
	return timerHandle;
}

TimerHandle CreateWallClockTimerWithoutThread(ThreadlessTimerEngine* m_threadlessTimerEngine,
										      const char* timerName,
									 		  u32 timeoutPeriodMinutes,
							  		 		  void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
									 		  TimerStart startNow,
									 		  TimerMode timerMode,
									 		  Bool traceTimerEvents)
{
	ENTRY;
	TimerHandle timerHandle = NULL_TIMER;
	if (timeoutPeriodMinutes > 0 && timeoutPeriodMinutes <= (24*60))
	{
		timerHandle = CreateAnyTimerWithoutThread(m_threadlessTimerEngine,
		 										  timerName,
		 										  timeoutPeriodMinutes*60*1000,
		 										  NotifyTimerExpiry,
		 										  startNow,
		 										  timerMode,
		 										  TIMER_WALL_CLOCK_ALIGNED,
		 										  traceTimerEvents);
	}
	
	Exit;
	return timerHandle;
}

TimerHandle CreateAnyTimerWithoutThread(ThreadlessTimerEngine* m_threadlessTimerEngine,
										const char* timerName,
										u32 timeoutPeriodMs,
										void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
										TimerStart startNow,
										TimerMode timerMode,
										TimerAlignment timerAlignment,
										Bool traceTimerEvents)
{
	ENTRY;

	Timer* timer;
	if (map_size(m_threadlessTimerEngine->m_theTimers) != MAX_TIMERS)
	{
		
		int findTimer = 1;
		do{
			timer = TimerCreate(timeoutPeriodMs,
								   timerMode,
								   timerName,
								   NotifyTimerExpiry,
								   timerAlignment,
								   traceTimerEvents);
			map_iterator_t it = map_find(m_threadlessTimerEngine->m_theTimers,&timer->m_timerHandle);
			if (!iterator_equal(it,map_end(m_threadlessTimerEngine->m_theTimers)))
			{
				findTimer = 1;
				TimerDestroy(&timer);
			}
				else findTimer = 0;
		}while(findTimer != 0);

		printf("print timer: timerName = %s, timerHandle = %u, timerMode = %d\n",timer->m_timerName, timer->m_timerHandle,timer->m_timerMode);
		*(Timer*)map_at(m_threadlessTimerEngine->m_theTimers,&timer->m_timerHandle) = *timer;

		if (startNow == TIMER_START_NOW)
		{
			ThreadlessStartTimer(m_threadlessTimerEngine,timer->m_timerHandle);
		}

	}
	Exit;
	return timer->m_timerHandle;
}

void ThreadlessStartTimer(ThreadlessTimerEngine* m_threadlessTimerEngine, TimerHandle timerHandle)
{
	ENTRY;
	ThreadlessStopTimer(m_threadlessTimerEngine, timerHandle);

	Timer* findTimer = NULL;
	map_iterator_t it = map_find(m_threadlessTimerEngine->m_theTimers, &timerHandle);
	if (!iterator_equal(it, map_end(m_threadlessTimerEngine->m_theTimers)))
	{
		findTimer = (Timer*)pair_second((pair_t*)iterator_get_pointer(it));
	}

	if (findTimer)
	{
		u32 expiryDelta = 0;
		switch(findTimer->m_timerAlignment)
		{
			case TIMER_UNALOGNED:
				expiryDelta = findTimer->m_timerPeriodMs;
				break;
			case TIMER_WALL_CLOCK_ALIGNED:
				expiryDelta = CalculateWallClockExpiryTime(findTimer->m_timerPeriodMs);
				break;
			default:
				break;
		}

		uptime_t expiryTime = expiryDelta + GetMillisecs(m_threadlessTimerEngine->m_uptime);
		*(uptime_t*)map_at(m_threadlessTimerEngine->m_runningTimers,&timerHandle) = expiryTime;
		
		ThreadlessCheckHeadTimer(m_threadlessTimerEngine);
	}                
	else
	{
		printf("%s\n", "can't find the timer whitout thread");
	}

	Exit;
}

void ThreadlessStopTimer(ThreadlessTimerEngine* m_threadlessTimerEngine, TimerHandle timerHandle)
{
	ENTRY;

	map_iterator_t it = map_find(m_threadlessTimerEngine->m_runningTimers,&timerHandle);
	if (!iterator_equal(it,map_end(m_threadlessTimerEngine->m_runningTimers)))
	{
		map_erase_pos(m_threadlessTimerEngine->m_runningTimers,it);
		ThreadlessCheckHeadTimer(m_threadlessTimerEngine);
	} 

	Exit;
}

u32 ThreadlessCalculateWallClockExpiryTime(u32 timerPeriodMillisecs)
{
	ENTRY;

	printf("timerPeriodMillisecs is %d\n",timerPeriodMillisecs);
	struct timeval timeVal;
	struct timezone timeZone;

	gettimeofday(&timeVal,&timeZone);
	struct tm* tmVal = gmtime(&(timeVal.tv_sec)); // UTC time

	u32 millisecsSinceMidnight = (tmVal->tm_hour*60*60*1000) + 
								 (tmVal->tm_min*60*1000) +
								 (tmVal->tm_sec * 1000) +
								 (timeVal.tv_usec / 1000);
	u32 millisecsToMidnight = (24 * 60 *60 * 1000) - millisecsSinceMidnight;

	u32 timeout = millisecsToMidnight % timerPeriodMillisecs;

	printf("wall clock expiry time is %d\n",timeout);
	Exit;
	return timeout;
}

void ThreadlessSelectorCallBack(void* args)
{
	printf("call back\n");
	ThreadlessTimerEngine* m_threadlessTimerEngine = (ThreadlessTimerEngine*)args;
	ThreadlessCheckTimers(m_threadlessTimerEngine);

	ThreadlessDeleteOrRestartExpiryTimers(m_threadlessTimerEngine);
}

void ThreadlessCheckTimers(ThreadlessTimerEngine* m_threadlessTimerEngine)
{
	ENTRY;

	list_clear(m_threadlessTimerEngine->m_expiredTimers);
	uptime_t timeNow = GetMillisecs(m_threadlessTimerEngine->m_uptime);

	map_iterator_t it;
	for (it = map_begin(m_threadlessTimerEngine->m_runningTimers); !iterator_equal(it,map_end(m_threadlessTimerEngine->m_runningTimers)); it = iterator_next(it))
	{
		if(timeNow >= *(uptime_t*)pair_second((pair_t*)iterator_get_pointer(it)))
		{
			TimerHandle handle = *(TimerHandle*)pair_first((pair_t*)iterator_get_pointer(it));
			map_iterator_t it_pos = map_find(m_threadlessTimerEngine->m_theTimers,&handle);
			if(iterator_equal(it_pos, map_end(m_threadlessTimerEngine->m_theTimers)))
			{
				printf("can't find timer\n");
				exit(1);
			}

			Timer timer = *(Timer*)pair_second((pair_t*)iterator_get_pointer(it_pos));
			printf("print timer: timerName = %s, timerHandle = %u, timerMode = %d\n",timer.m_timerName, timer.m_timerHandle,timer.m_timerMode);
			list_push_back(m_threadlessTimerEngine->m_expiredTimers, &timer);
		}

	}

	Exit;
}

void ThreadlessDeleteOrRestartExpiryTimers(ThreadlessTimerEngine* m_threadlessTimerEngine)
{
	ENTRY;

	list_iterator_t it;
	for (it = list_begin(m_threadlessTimerEngine->m_expiredTimers); !iterator_equal(it, list_end(m_threadlessTimerEngine->m_expiredTimers)); it = iterator_next(it))
	{
		Timer timer = *(Timer*)iterator_get_pointer(it);
		printf("print timer: timerName = %s, timerHandle = %u, timerMode = %d\n",timer.m_timerName, timer.m_timerHandle,timer.m_timerMode);
		if (timer.m_timerMode == TIMER_SINGLE_SHOT)
		{
			printf("delete timer\n");
			ThreadlessDeleteTimer(m_threadlessTimerEngine, timer.m_timerHandle);
		}
		else if (timer.m_timerMode == TIMER_REPEAT)
		{
			printf("timer repeat\n");
			StartTimer(timer.m_timerHandle);
		}
		else
		{
			printf("stop timer wait user to delete it\n");
			ThreadlessStopTimer(m_threadlessTimerEngine,timer.m_timerHandle);
		}

	}

	for (it = list_begin(m_threadlessTimerEngine->m_expiredTimers); !iterator_equal(it, list_end(m_threadlessTimerEngine->m_expiredTimers)); it = iterator_next(it))
	{
		Timer timer = *(Timer*)iterator_get_pointer(it);
		timer.NotifyTimerExpiry(timer.m_timerHandle);	
	}

	Exit;
}

void ThreadlessDeleteTimer(ThreadlessTimerEngine* m_threadlessTimerEngine, TimerHandle timerHandle)
{
	ENTRY;

	if (timerHandle == NULL_TIMER)
	{
		printf("timer handle is NULL_TIMER, delete failed\n");
		Exit;
		return;
	}

	map_iterator_t it  = map_find(m_threadlessTimerEngine->m_theTimers, &timerHandle);
	if (!iterator_equal(it, map_end(m_threadlessTimerEngine->m_theTimers)))
	{
		map_erase_pos(m_threadlessTimerEngine->m_theTimers,it);
		printf("find the timer and delete it\n");
	}

	Exit;
}

void ThreadlessCheckHeadTimer(ThreadlessTimerEngine* m_threadlessTimerEngine)
{ 
	ENTRY;
	TimerHandle timerHandle = NULL_TIMER;
	map_iterator_t it;
	uptime_t min = -1;
	for (it = map_begin(m_threadlessTimerEngine->m_runningTimers); !iterator_equal(it,map_end(m_threadlessTimerEngine->m_runningTimers)); it = iterator_next(it))
	{
		if (min == -1)
		{
			min = *(uptime_t*)pair_second((pair_t*)iterator_get_pointer(it));
		}
		else
		{
			if (min > *(uptime_t*)pair_second((pair_t*)iterator_get_pointer(it)) )
			{
				min =  *(uptime_t*)pair_second((pair_t*)iterator_get_pointer(it));
			}
		}
	}

	if (min != -1)
	{
		uptime_t now = GetMillisecs(m_threadlessTimerEngine->m_uptime);
		uptime_t timeout = now > min ? now : min;
		u32 relativeTimeout = timeout - now;

		RegisterTimeout(m_threadlessTimerEngine->m_selector, relativeTimeout,ThreadlessSelectorCallBack, (void*)m_threadlessTimerEngine);
	}
}
