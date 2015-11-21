// Scheduler.h: object definitions for generic scheduler
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"

template<typename proc_T> class Scheduler;
typedef Scheduler<Process> GenericScheduler;

template <typename proc_T>
class Scheduler {
public:
  virtual proc_T *NextProcess() = 0;
protected:
  explicit Scheduler() {}
  virtual ~Scheduler() {}
};


class SchedulerTimeQuantum {
protected:
  SchedulerTimeQuantum(uint time_q)
    : m_time_quantum(time_q)
  {}
  uint m_time_quantum;
};

#endif
