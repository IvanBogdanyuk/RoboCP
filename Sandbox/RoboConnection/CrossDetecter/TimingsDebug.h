#pragma once

// отладочный замер времени
#ifdef DEBUG_HELI_TIMINGS
#include <QtCore\qelapsedtimer.h>
#define DEBUG_TIMER_VAR __QET
#define DEBUG_TIME_ELAPSED_VAR __te
#define TimerCreate() QElapsedTimer DEBUG_TIMER_VAR
#define TimerUpdate() DEBUG_TIMER_VAR.start()
#define TimerElapsed() qint64 DEBUG_TIME_ELAPSED_VAR = DEBUG_TIMER_VAR.elapsed(); if(DEBUG_TIME_ELAPSED_VAR>0) printf("Thread %s ran for %i ms\n", __FILE__, DEBUG_TIME_ELAPSED_VAR)
#else
#define TimerCreate()
#define TimerUpdate()
#define TimerElapsed()
#endif