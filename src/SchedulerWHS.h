// SchedulerWHS.h: object definition for SchedulerWHS
#ifndef SCHEDULER_WHS_H
#define SCHEDULER_WHS_H

#include "Scheduler.h"


class SchedulerWHS: public Scheduler<ProcessWHS>, SchedulerTimeQuantum {
public:
  SchedulerWHS(uint time_q)
    : SchedulerTimeQuantum(time_q)
  {}
  virtual ProcessWHS *NextProcess();
};

#endif
