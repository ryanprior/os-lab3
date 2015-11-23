// SchedulerMFQS.h: object definition for SchedulerMFQS
#ifndef SCHEDULER_MFQS_H
#define SCHEDULER_MFQS_H

#include "Scheduler.h"
#include <vector>
#include <deque>
#include <set>


class SchedulerMFQS : public Scheduler<ProcessMFQS>, SchedulerTimeQuantum {
public:
  SchedulerMFQS(uint num_queues,
                uint time_q)
    : SchedulerTimeQuantum(time_q),
      m_queues(num_queues),
      m_processes_by_age()
  {}
  virtual ~SchedulerMFQS();
  virtual ProcessMFQS *NextProcess();
  virtual void Add(uint cpu_time, ProcessMFQS *proc);
  virtual uint NextEventTime(uint cpu_time);
  virtual void DispatchEvent(uint cpu_time);

protected:
  std::vector<std::deque<ProcessMFQS*> > m_queues;
  std::set<ProcessMFQS*> m_processes_by_age;
};

#endif
