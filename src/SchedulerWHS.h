// SchedulerWHS.h: object definition for SchedulerWHS
#ifndef SCHEDULER_WHS_H
#define SCHEDULER_WHS_H

#include "Scheduler.h"
#include <set>


class SchedulerWHS: public Scheduler<ProcessWHS>,
                    SchedulerTimeQuantum<ProcessWHS>,
                    SchedulerAge<ProcessWHS>
{
public:
  SchedulerWHS(uint time_q, uint age_time)
    : SchedulerTimeQuantum(time_q),
      SchedulerAge(age_time)
  {}
  virtual ~SchedulerWHS() {}
  virtual void Add(uint cpu_time, ProcessWHS *proc);
  virtual uint NextEventTime(uint cpu_time) const;
  virtual void DispatchEvent(uint cpu_time);
  virtual void AdvanceTime(uint old_time, uint new_time);
  virtual bool Empty() const;
protected:
  virtual void handle_proc_tq_expire(ProcessWHS *proc);
  virtual void handle_proc_stop(ProcessWHS *proc);
  void handle_proc_dispose(ProcessWHS *proc);
  virtual void run_proc(ProcessWHS *proc, uint run_time, uint cpu_time);
  ProcessWHS *top_priority() const { return *m_processes_by_priority.begin(); }
  std::set<ProcessWHS*, ProcessWHS::compare_by_priority> m_processes_by_priority;
  std::set<ProcessWHS*, ProcessWHS::compare_by_age> m_processes_by_age;
};

#endif
