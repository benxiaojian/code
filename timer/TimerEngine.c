#include "TimerEngine.h"

TimerEngine* GetTimeEngineInstance()
{
	return m_timerEngine;
}

TimerEngineThread* GetTimeEngineThreadInstance()
{
	return m_timerEngineThread;
}

TimerEngine* TimerEngineCreate(Mutex* mutex, Condition* condition)
{
	ENTRY;
	if (m_timerEngine) return m_timerEngine;

	// TimerEngine* m_timerEngine = (TimerEngine*)malloc(sizeof(TimerEngine));
	m_timerEngine = (TimerEngine*)malloc(sizeof(TimerEngine));

	if (!m_timerEngine)
	{
		#ifdef DEBUG
		printf("timerEngine malloc failed\n");
		#endif

		return NULL;
	}

	// m_timerEngine->m_timersMutex = (Mutex*)malloc(sizeof(Mutex));
	// m_timerEngine->m_timersCondition = (Condition*)malloc(sizeof(Condition));
	m_timerEngine->m_timersMutex = mutex;
	m_timerEngine->m_timersCondition = condition;

	m_timerEngine->m_uptime = UpTimeCreate(FALSE);
	
	type_register(TimerHandle, _TimerHandle_init, _TimerHandle_copy, _TimerHandle_less, _TimerHandle_destroy);
	type_register(uptime_t, _uptime_t_init, _uptime_t_copy, _uptime_t_less, _uptime_t_destroy);
	type_register(Timer, _Timer_init, _Timer_copy, _Timer_less, _Timer_destroy);
	// m_timerEngine->m_theTimers = create_map(TimerHandle, Timer*);
	m_timerEngine->m_theTimers = create_map(TimerHandle, Timer);
	m_timerEngine->m_runningTimers = create_map(TimerHandle,uptime_t);
	m_timerEngine->m_expiredTimers = create_list(Timer);
	map_init(m_timerEngine->m_theTimers);
	map_init(m_timerEngine->m_runningTimers);
	list_init(m_timerEngine->m_expiredTimers);

	return m_timerEngine;
}

void TimerEngineDestroy()
{
	// free(m_timerEngine->m_timersMutex);
	// free(m_timerEngine->m_timersCondition);
	m_timerEngine->m_timersMutex = NULL;
	m_timerEngine->m_timersCondition = NULL;

	UpTimeDestroy(&m_timerEngine->m_uptime);

	map_destroy(m_timerEngine->m_theTimers);
	map_destroy(m_timerEngine->m_runningTimers);
	list_destroy(m_timerEngine->m_expiredTimers);

	free(m_timerEngine);
	m_timerEngine = NULL;
}

TimerHandle CreateTimer(const char* timerName,
						u32 timeoutPeriodMs,
						void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
						TimerStart startNow,
						TimerMode timerMode,
						Bool traceTimerEvents)
{
	ENTRY;
	TimerAlignment timerAlignment = TIMER_UNALOGNED;
	printf("timer mode is %d\n",timerMode);
	TimerHandle timerHandle = CreateAnyTimer(timerName,
											 timeoutPeriodMs,
											 NotifyTimerExpiry,
											 startNow,
											 timerMode,
											 timerAlignment,
											 traceTimerEvents
											 );
	return timerHandle;
}

// create timer about the clock, the expiry time is minute
TimerHandle CreateWallClockTimer(const char* timerName,
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
		timerHandle = CreateAnyTimer(timerName,
									 timeoutPeriodMinutes * 60 * 1000,
									 NotifyTimerExpiry,
									 startNow,
									 timerMode,
									 TIMER_WALL_CLOCK_ALIGNED,
									 traceTimerEvents);
	}

	Exit;
	return timerHandle;
}


static TimerHandle CreateAnyTimer(	const char* timerName,
						   	u32 timeoutPeriodMs,
							void (*NotifyTimerExpiry)(TimerHandle expiredTimer),
							TimerStart startNow,
							TimerMode timerMode,
							TimerAlignment timerAlignment,
							Bool traceTimerEvents)
{
	ENTRY;
	if (map_size(m_timerEngine->m_theTimers) != MAX_TIMERS)
	{
		Timer* timer = NULL;
		LockMutex(m_timerEngine->m_timersMutex);
		// lock mutex

		int findTimer = 1;
		do{
			timer = TimerCreate(timeoutPeriodMs,
								timerMode,
								timerName,
								NotifyTimerExpiry,
								timerAlignment,
								traceTimerEvents);
			if (FindTimer(m_timerEngine->m_theTimers,&timer->m_timerHandle) != NULL)
			//map_iterator_t it = map_find(m_timerEngine->m_theTimers,timer->m_timerHandle);
			//if (!iterator_equal(it,map_end(m_timerEngine->m_theTimers)))
			{
				findTimer = 1;
				TimerDestroy(&timer);
			}
			else findTimer = 0;

		}while (findTimer != 0);

		printf("insert timer into timer map <timerHandle, timer>\n");
		*(Timer*)map_at(m_timerEngine->m_theTimers, &timer->m_timerHandle) = *timer;
		printf("timer mode is %d\n",timer->m_timerMode);
		printf("print timer: timerName = %s, timerHandle = %u, timerMode = %d\n",timer->m_timerName, timer->m_timerHandle,timer->m_timerMode);

		//unlock mutex
		UnlockMutex(m_timerEngine->m_timersMutex);
		if (startNow == TIMER_START_NOW)
		{
			StartTimer(timer->m_timerHandle);
		}

		Exit;
		return timer->m_timerHandle;
	}
	else
	{
		#ifdef DEBUG
		printf("timerEngine create too many timers, create timer failed\n");
		#endif

		Exit;
		return NULL_TIMER;
	}
}

void StartTimer(TimerHandle m_timerHandle)
{
	ENTRY;
	StopTimer(m_timerHandle);

	Timer* findTimer = NULL;
	LockMutex(m_timerEngine->m_timersMutex);
	findTimer = FindTimer(m_timerEngine->m_theTimers, &m_timerHandle);
	UnlockMutex(m_timerEngine->m_timersMutex);

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

		uptime_t expiryTime = expiryDelta + GetMillisecs(m_timerEngine->m_uptime);

		LockMutex(m_timerEngine->m_timersMutex);

		*(uptime_t*)map_at(m_timerEngine->m_runningTimers, &m_timerHandle) = expiryTime;
		ConditionSignal(m_timerEngine->m_timersCondition);

		UnlockMutex(m_timerEngine->m_timersMutex);
	}

	Exit;
}

void StopTimer(TimerHandle m_timerHandle)
{
	ENTRY;
	LockMutex(m_timerEngine->m_timersMutex);
	
	map_iterator_t it = map_find(m_timerEngine->m_runningTimers, &m_timerHandle);
	if (!iterator_equal(it, map_end(m_timerEngine->m_runningTimers)))
	{
		map_erase_pos(m_timerEngine->m_runningTimers,it);
		ConditionSignal(m_timerEngine->m_timersCondition);
	}

	Exit;
	UnlockMutex(m_timerEngine->m_timersMutex);	
}

Timer* FindTimer(map_t* map, TimerHandle* key)
{
	ENTRY;
	map_iterator_t it_pos = map_find(map,key);
	if (!iterator_equal(it_pos, map_end(map)))
	{
		return (Timer*)pair_second(iterator_get_pointer(it_pos));
	}

	Exit;
	return NULL;
}

u32 CalculateWallClockExpiryTime(u32 timerPeriodMillisecs)
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

TimerEngineThread* TimerEngineThreadCreate()
{
	ENTRY;
	if (m_timerEngineThread) return NULL;
	m_timerEngineThread = (TimerEngineThread*)malloc(sizeof(TimerEngineThread));
	if (!m_timerEngineThread)
	{
		#ifdef DEBUG
		printf("timer engine thread malloc failed\n");
		#endif

		return NULL;
	}

	m_timerEngineThread->thread = ThreadCreate("timerEngineThread");
	m_timerEngineThread->thread->ThreadProcedure = TimerEngineThreadProcedure;
	InitialiseMutex(&m_timerEngineThread->m_timersMutex);
	ConditionInit(&m_timerEngineThread->m_timersCondition);

	Exit;
	return m_timerEngineThread;
}

void TimerEngineThreadDestroy()
{
	ThreadDestroy(&(m_timerEngineThread->thread));
	ConditionDestroy(&m_timerEngineThread->m_timersCondition);
	DestroyMutex(&m_timerEngineThread->m_timersMutex);

	free(m_timerEngineThread);
	m_timerEngineThread = NULL;
}

u32 TimerEngineThreadProcedure()
{
	ENTRY;
	while(!(m_timerEngineThread->thread->m_threadStopRequest))
	{
		LockMutex(&m_timerEngineThread->m_timersMutex);

		struct timespec absTimeout;
		u32 millisecsToNextTimerExpiry = GetMillisecsToNextTimerExpiry();
		
		ConditionGetAbsTimeout(millisecsToNextTimerExpiry, &absTimeout);

		ConditionTimedWait(m_timerEngine->m_timersCondition, m_timerEngine->m_timersMutex, &absTimeout);

		CheckTimer(); 

		UnlockMutex(&m_timerEngineThread->m_timersMutex);

		DeleteOrRestartExpiredTimers();
	}

	Exit;
}

void StartTimerEngineThread()
{
	ENTRY;
	m_timerEngine->m_timersMutex = &m_timerEngineThread->m_timersMutex;
	m_timerEngine->m_timersCondition = &m_timerEngineThread->m_timersCondition;

	u32 threadStackSizeBytes = 63*2014;
	StartThread(m_timerEngineThread->thread,threadStackSizeBytes);
	Exit;
}

// find the next expiry timer, the min expity timer
u32 GetMillisecsToNextTimerExpiry()
{
	ENTRY;
	u32 timeoutMilliSecs = 5000;

	// ?????
	// the min time of condition waiting.
	// must find the min time....but <timerHandle, uptime_t> can't find, must sort
	// radisys use multimap<uptime_t, timerHandle>, the first is min....
	
	// map_iterator_t it = map_begin(m_timerEngine->m_runningTimers);
	// if (!iterator_equal(it, map_end(m_timerEngine->m_runningTimers)))
	// {
	// 	uptime_t timeNow = GetTickCountMs();
		
	// 	if ((*(uptime_t*)pair_second(iterator_get_pointer(it))) > timeNow)
	// 	{
	// 		timeoutMilliSecs = (*(uptime_t*)pair_second(iterator_get_pointer(it))) - timeNow;
	// 	}
	// 	else
	// 	{
	// 		u32 arbitraryShortTime = 1000;
	// 		if ((*(uptime_t*)pair_second(iterator_get_pointer(it)) + arbitraryShortTime) <= timeNow)
	// 		{
	// 			#ifdef DEBUG
	// 			printf("....i don't know print what...so sad\n");
	// 			#endif
	// 		}

	// 		timeoutMilliSecs = arbitraryShortTime;
	// 	}
	// }

	map_iterator_t it;
	uptime_t mintime = -1;
	for (it = map_begin(m_timerEngine->m_runningTimers); !iterator_equal(it,map_end(m_timerEngine->m_runningTimers)); it = iterator_next(it))
	{
		if (mintime == -1)
			mintime = *(uptime_t*)pair_second(iterator_get_pointer(it)) ;
		else if ((*(uptime_t*)pair_second(iterator_get_pointer(it))) < mintime)
			mintime = *(uptime_t*)pair_second(iterator_get_pointer(it)) ;
	}

	uptime_t timeNow = GetTickCountMs();
	if (mintime > timeNow)
	{
		timeoutMilliSecs = mintime - timeNow;
	} 
	else
	{
		u32 arbitraryShortTime = 1000;
		if ((mintime + arbitraryShortTime) <= timeNow)
		{
			#ifdef DEBUG
			printf("....i don't know print what...so sad\n");
			#endif
		}

		timeoutMilliSecs = arbitraryShortTime;
	}

	Exit;
	return timeoutMilliSecs;


}

uptime_t GetTickCountMs()
{
	UpTime* uptime = UpTimeCreate(FALSE);
	uptime_t nowTime = GetMillisecs(uptime);
	UpTimeDestroy(&uptime);
	printf("time now is %llu\n",nowTime);
	return nowTime;
}

void CheckTimer()
{
	ENTRY;
	list_clear(m_timerEngine->m_expiredTimers);

	uptime_t timeNow = GetTickCountMs();
	map_iterator_t it;
	for (it = map_begin(m_timerEngine->m_runningTimers); !iterator_equal(it, map_end(m_timerEngine->m_runningTimers)); it = iterator_next(it))
	{
		uptime_t timerTime = *(uptime_t*)pair_second(iterator_get_pointer(it));
		// the timer expire
		if (timeNow >= timerTime)
		{
			TimerHandle handle = *(TimerHandle*)pair_first(iterator_get_pointer(it));
			map_iterator_t it_pos = map_find(m_timerEngine->m_theTimers, &handle);
			if (iterator_equal(it_pos, map_end(m_timerEngine->m_theTimers))) exit(1);
			Timer timer = *(Timer*)pair_second(iterator_get_pointer(it_pos));
			printf("add timer into list<expire timer>\n");
			printf("print timer: timerName = %s, timerHandle = %u, timerMode = %d\n",timer.m_timerName, timer.m_timerHandle,timer.m_timerMode);
			list_push_back(m_timerEngine->m_expiredTimers, &timer);
		}
		else
		{
			continue;
		}
	}// end for

	Exit;
}

void DeleteTimer(TimerHandle timerHandle)
{
	ENTRY;

	if (timerHandle == NULL_TIMER)
	{
		printf("timerhandle is NULL_TIMER, delete it failed\n");
		Exit;
		return;
	}

	map_iterator_t it = map_find(m_timerEngine->m_theTimers,timerHandle);
	if (!iterator_equal(it, map_end(m_timerEngine->m_theTimers)))
	{
		printf("find the timer and delete it\n");
		map_erase_pos(m_timerEngine->m_theTimers, it);
	}

	Exit;
}

void DeleteOrRestartExpiredTimers()
{
	ENTRY;
	list_iterator_t it;
	for (it = list_begin(m_timerEngine->m_expiredTimers); !iterator_equal(it, list_end(m_timerEngine->m_expiredTimers)); it = iterator_next(it))
	{
		Timer timer = *(Timer*)iterator_get_pointer(it);

		printf("print timer: timerName = %s, timerHandle = %u, timerMode = %d\n",timer.m_timerName, timer.m_timerHandle,timer.m_timerMode);
		printf("timer mode is %d\n",timer.m_timerMode);
		if (timer.m_timerMode == TIMER_SINGLE_SHOT)
		{
			printf("delete timer\n");
			DeleteTimer(timer.m_timerHandle);
		}
		else if (timer.m_timerMode == TIMER_REPEAT)
		{
			printf("timer repeat\n");
			StartTimer(timer.m_timerHandle);
		}
		else
		{
			printf("stop timer wait user to delete it\n");
			StopTimer(timer.m_timerHandle);
		}
	}//end for

	for (it = list_begin(m_timerEngine->m_expiredTimers); !iterator_equal(it, list_end(m_timerEngine->m_expiredTimers)); it = iterator_next(it))
	{
		printf("call back to func NotifyTimerExpiry\n");
		Timer timer = *(Timer*)iterator_get_pointer(it);
		timer.NotifyTimerExpiry(timer.m_timerHandle);	
	}

	Exit;
}