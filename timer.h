#ifndef TIMER_H
#define TIMER_H

typedef struct Timer {
  int counter;
  int end;
  int counting;
} Timer;

typedef enum TimerStatus { NotFinished = 0, Finished, Idle } TimerStatus;

void Timer_Start(Timer *timer, int tickCount);
void Timer_Clear(Timer *timer);
TimerStatus Timer_Update(Timer *timer);

#endif
