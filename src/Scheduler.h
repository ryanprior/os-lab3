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
  virtual ~Scheduler() {}
  virtual void Add(uint cpu_time, process_T *proc) = 0;
  virtual uint NextEventTime(uint cpu_time) const = 0;
  virtual void DispatchEvent(uint cpu_time) = 0;
  virtual void AdvanceTime(uint old_time, uint new_time) = 0;
  virtual bool empty() const = 0;
  virtual void handle_proc_stop(process_T *proc) = 0;
protected:
  explicit Scheduler()
    : m_proc_running(false)
  {}
  bool m_proc_running;
};


template <class process_T>
class SchedulerTimeQuantum {
protected:
  SchedulerTimeQuantum(uint time_q)
    : m_time_quantum(time_q),
      m_tq_remaining(time_q)
  {}
  virtual void handle_proc_tq_expire(process_T *proc) = 0;
  const uint m_time_quantum;
  uint m_tq_remaining;
};


template <class process_T>
class SchedulerAge {
protected:
  SchedulerAge(uint age_time)
    : m_age_time(age_time)
  {}
  virtual process_T* oldest_proc() const = 0;
  virtual void age_process(process_T *proc, uint cpu_time) = 0;
  const uint m_age_time;
};

#endif
