// SchedulerRTS.h: object definition for SchedulerRTS
#ifndef SCHEDULER_RTS_H
#define SCHEDULER_RTS_H

#include "Scheduler.h"
#include <deque>


class SchedulerRTS : public Scheduler<ProcessRTS> {
public:
	SchedulerRTS(bool hardmode)
    r_queue,
    hardmode,
  {}
  virtual ~SchedulerRTS() {}
  virtual ProcessRTS *NextProcess();
  
protected:
  bool hardmode;
  std::deque<ProcessMFQS*> r_queue
};

#endif
