// SchedulerRTS.h: object definition for SchedulerRTS
#ifndef SCHEDULER_RTS_H
#define SCHEDULER_RTS_H

#include "Scheduler.h"


class SchedulerRTS : public Scheduler<ProcessRTS> {
public:
  virtual ~SchedulerRTS() {}
  virtual ProcessRTS *NextProcess();
};

#endif
