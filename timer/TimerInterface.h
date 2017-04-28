#ifndef __TimerInterface_h__
#define __TimerInterface_h__

#include <RsysConstAndMacros.h>
#include <string.h>
#include "UpTime.h"

// timer id
typedef u32 TimerHandle;

static const TimerHandle NULL_TIMER = 0;

// timer start mode
typedef enum{
	TIMER_START_NOW,   // start timer now
	TIMER_WAIT		   // start when user call by startTimer()
}TimerStart;

// timer end mode
typedef enum 
{
	TIMER_SINGLE_SHOT,   // single shot timer are deleted on expiry
	TIMER_SINGLE_SHOT_WITHOUT_DELETE, // will not deleted, must user to delete
	TIMER_REPEAT // will repeat when timer expiry
}TimerMode;

typedef enum
{
	TIMER_UNALOGNED,
	TIMER_WALL_CLOCK_ALIGNED
}TimerAlignment;

static void _TimerHandle_init(const void* cpv_input, void* pv_output)
{
	*(TimerHandle*)cpv_input = 0;
	*(bool_t*)pv_output = true;
}

static void _TimerHandle_destroy(const void* cpv_input, void* pv_output)
{
	*(TimerHandle*)cpv_input = 0;
	*(bool_t*)pv_output = true;
}

static void _TimerHandle_copy(const void* cpv_first, const void* cpv_second, void* pv_output)
{
	*(TimerHandle*)cpv_first = *(TimerHandle*)cpv_second;
	*(bool_t*)pv_output = true;
}

static void _TimerHandle_less(const void* cpv_first, const void* cpv_second, void* pv_output)
{
	*(bool_t*)pv_output = *(TimerHandle*)cpv_first < *(TimerHandle*)cpv_second ? true : false;
}


#endif