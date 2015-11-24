// SchedulerRTS.h: object definition for SchedulerRTS
#ifndef SCHEDULER_RTS_H
#define SCHEDULER_RTS_H

#include "Scheduler.h"
#include <set>


class SchedulerRTS : public Scheduler<ProcessRTS> {
public:
  SchedulerRTS(bool hardmode)
    : r_queue(),
      r_hardmode(hardmode)
  {}
  virtual ~SchedulerRTS();
  virtual void Add(uint cpu_time, ProcessRTS *proc);
  virtual ProcessRTS *NextProcess();
  virtual uint NextEventTime(uint cpu_time) const;
  virtual void DispatchEvent(uint cpu_time);
  virtual void AdvanceTime(uint old_time, uint new_time);
  virtual bool Empty() const;
protected:
  virtual void handle_proc_stop(ProcessRTS *proc);
  virtual void run_proc(ProcessRTS *proc, uint run_time, uint cpu_time);
  const bool r_hardmode;
  std::set<ProcessRTS*, ProcessRTS::compare_by_deadline> r_queue;
};

#endif
