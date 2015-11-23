// Scheduler.h: object definitions for generic scheduler
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include <Signal.h>


template <class process_T>
class Scheduler {
public:
  Gallant::Signal2<process_T*, uint> changes_queue; // Signals with
                                                    // process and new
                                                    // queue.
  Gallant::Signal1<process_T*> ages; // Signals when process age timer
                                     // expires.
  virtual process_T *NextProcess() = 0;
  virtual ~Scheduler() {}
  virtual void Add(uint cpu_time, process_T *proc) = 0;
  virtual uint NextEventTime(uint cpu_time) = 0;
  virtual void DispatchEvent(uint cpu_time) = 0;
  const virtual inline bool preemptive() const { return false; }
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
