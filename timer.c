#include "timer.h"

void Timer_Start(Timer *timer, int tickCount) {
  timer->end = tickCount;
  timer->counter = 0;
  timer->counting = 1;
}

void Timer_Clear(Timer *timer) {
  timer->end = 0;
  timer->counting = 0;
}

TimerStatus Timer_Update(Timer *timer) {
  if (!timer->counting)
    return Idle;

  if (timer->counter < timer->end) {
    timer->counter++;
    return NotFinished;
  }

  if (timer->counting)
    timer->counting = 0;
  return Finished;
}
