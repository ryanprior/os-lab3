// Scheduler.h: object definitions for generic scheduler
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"

template<class process_T> class Scheduler;
typedef Scheduler<Process> GenericScheduler;

template <class process_T>
class Scheduler {
public:
  virtual process_T *NextProcess() = 0;
  virtual ~Scheduler() {}
  virtual void Add(process_T *proc) = 0;
protected:
  explicit Scheduler() {}
};


class SchedulerTimeQuantum {
protected:
  SchedulerTimeQuantum(uint time_q)
    : m_time_quantum(time_q)
  {}
  uint m_time_quantum;
};

#endif
