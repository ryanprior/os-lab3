// SchedulerMFQS.h: object definition for SchedulerMFQS
#ifndef SCHEDULER_MFQS_H
#define SCHEDULER_MFQS_H

#include "Scheduler.h"
#include <vector>
#include <deque>
#include <set>
#include <map>


class SchedulerMFQS : public Scheduler<ProcessMFQS>,
                      SchedulerTimeQuantum<ProcessMFQS>,
                      SchedulerAge<ProcessMFQS> {
public:
  SchedulerMFQS(uint num_queues,
                uint time_q,
                uint age_time)
    : SchedulerTimeQuantum(time_q),
      SchedulerAge(age_time),
      m_queues(num_queues),
      m_processes_by_age(),
      m_proc_queues()
  {}
  virtual ~SchedulerMFQS() {}
  virtual void Add(uint cpu_time, ProcessMFQS *proc);
  virtual uint NextEventTime(uint cpu_time) const;
  virtual void DispatchEvent(uint cpu_time);
  virtual void AdvanceTime(uint old_time, uint new_time);
  virtual bool empty() const { return m_processes_by_age.empty(); }
protected:
  int first_non_empty_queue_index() const; // -1 if all empty
  inline const uint time_q_for(uint queue) const;
  virtual ProcessMFQS *oldest_proc() const;
  void move_process_to(ProcessMFQS *proc, uint new_queue);
  virtual void age_process(ProcessMFQS *proc, uint cpu_time);
  virtual void handle_proc_tq_expire(ProcessMFQS *proc);
  virtual void handle_proc_stop(ProcessMFQS *proc);
  void handle_proc_dispose(ProcessMFQS *proc);
  std::vector<std::deque<ProcessMFQS*> > m_queues;
  std::set<ProcessMFQS*, ProcessMFQS::compare_by_age> m_processes_by_age;
  std::map<ProcessMFQS*, uint> m_proc_queues;
};

#endif
