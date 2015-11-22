// SchedulerMFQS.h: object definition for SchedulerMFQS
#ifndef SCHEDULER_MFQS_H
#define SCHEDULER_MFQS_H

#include "Scheduler.h"
#include <vector>
#include <deque>


class SchedulerMFQS : public Scheduler<ProcessMFQS>, SchedulerTimeQuantum {
public:
  SchedulerMFQS(uint num_queues,
                uint time_q)
    : SchedulerTimeQuantum(time_q),
      m_queues(num_queues)
  {}
  virtual ~SchedulerMFQS();
  virtual ProcessMFQS *NextProcess();
  virtual void Add(ProcessMFQS *proc);
protected:
  std::vector<std::deque<ProcessMFQS*> > m_queues;
};

#endif
