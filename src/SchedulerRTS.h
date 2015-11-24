// SchedulerRTS.h: object definition for SchedulerRTS
#ifndef SCHEDULER_RTS_H
#define SCHEDULER_RTS_H

#include "Scheduler.h"
#include <set>


class SchedulerRTS : public Scheduler<ProcessRTS> {
public:
	SchedulerRTS(bool hardmode)
    r_queue,
    hardmode,
  {}
  virtual ~SchedulerRTS() {}
  virtual void Add(uint cpu_time, ProcessMFQS *proc);
  virtual ProcessRTS *NextProcess();
  virtual uint NextEventTime(uint cpu_time) const;
  virtual void DispatchEvent(uint cpu_time);
protected:
  bool hardmode;
  std::set<ProcessMFQS*, ProcessMFQS::compare_by_deadline> r_queue
};

#endif
