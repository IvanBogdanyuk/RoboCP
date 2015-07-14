// illegal things happen below
#ifdef DEBUG_HELI_TIMINGS
#include <QtCore\qelapsedtimer.h>
#define TimerCreate() QElapsedTimer __QET
#define TimerUpdate() __QET.start()
#define TimerElapsed() qint64 __te = __QET.elapsed(); if(__te>0) printf("Thread %s ran for %i ms\n", __FILE__, __te)
#else
#define TimerCreate()
#define TimerUpdate()
#define TimerElapsed()
#endif