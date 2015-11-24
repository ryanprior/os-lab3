#include "SchedulerMFQS.h"
#include <limits>
#include <iostream>

// SchedulerMFQS::~SchedulerMFQS() {
//   for(auto i=m_queues.begin(); i != m_queues.end(); ++i) {
//     while(!i->empty()) {
//       delete i->front();
//       i->pop_front();
//     }
//   }
// }

void SchedulerMFQS::Add(uint cpu_time, ProcessMFQS *proc) {
  proc->reset_last_cycle(cpu_time);
  m_queues.front().push_back(proc);
  m_processes_by_age.insert(proc);
  m_proc_queues[proc] = 0;
  proc->dispose.Connect(this, &SchedulerMFQS::handle_proc_dispose);
  proc->tq_expires.Connect(this, &SchedulerMFQS::handle_proc_tq_expire);
  if(!m_proc_running) {
    proc->Run(0, m_time_quantum, cpu_time);
    m_proc_running = proc;
  }
}

uint SchedulerMFQS::NextEventTime(uint cpu_time) const {
  const int max = std::numeric_limits<uint>::max();
  uint result = max;
  int i = first_non_empty_queue_index();
  if(i>=0) {
    result = m_queues[i].front()->time_remaining(cpu_time);
  }
  if(i < m_queues.size()) {
    result = std::min(result, time_q_for(i));
  }
  auto oldest = oldest_proc();
  if(oldest) {
    result = std::min(result, m_age_time + oldest->last_cycle() - cpu_time);
  }
  if(!m_proc_running && !empty()) {
    result = std::min(result, (uint)1);
  }
  if(result < max) {
    result += cpu_time;
  }
  return result;
}

void SchedulerMFQS::DispatchEvent(uint cpu_time) {
  int i = first_non_empty_queue_index();
  if(i>=0) {
    ProcessMFQS *proc = m_queues[i].front();
    if(!m_proc_running) {
      proc->Run(0, m_time_quantum, cpu_time);
      m_proc_running = true;
    } else {
      uint run_time = std::min(proc->time_remaining(cpu_time),
                               time_q_for(i));
      ProcessMFQS *oldest = oldest_proc();
      if(oldest
         && m_age_time + oldest->last_cycle() - cpu_time < run_time) {
        age_process(oldest, cpu_time);
      } else {
        proc->Run(m_tq_remaining, time_q_for(i), cpu_time);
      }
    }
  }
}

void SchedulerMFQS::AdvanceTime(uint old_time, uint new_time) {
  if(m_proc_running) {
    ProcessMFQS *proc = m_queues[first_non_empty_queue_index()].front();
    uint run_time = new_time - old_time;
    proc->Run(run_time, m_tq_remaining, new_time);
    m_tq_remaining -= run_time;
  }
}

int SchedulerMFQS::first_non_empty_queue_index() const {
  int result = -1;
  for(int i=0; result<0 && i != m_queues.size(); ++i) {
    if(!m_queues[i].empty()) {
      result = i;
    }
  }
  return result;
}
inline const uint SchedulerMFQS::time_q_for(uint queue) const {
  return m_time_quantum << queue;
}
ProcessMFQS *SchedulerMFQS::oldest_proc() const {
  ProcessMFQS *result = NULL;
  // find first process that's not in top or next-to-top queue
  for(auto oldest = m_processes_by_age.begin();
      !result
        && oldest != m_processes_by_age.end()
        && m_proc_queues.at(*oldest) > 1;
      ++oldest) {
    result = *oldest;
  }
  return result;
}

void SchedulerMFQS::move_process_to(ProcessMFQS *proc, uint new_queue) {
  uint queue_i = m_proc_queues[proc];
  m_queues[queue_i].pop_front();
  m_proc_queues[proc] = new_queue;
  m_queues[new_queue].push_back(proc);
  this->changes_queue(proc, new_queue);
}


void SchedulerMFQS::age_process(ProcessMFQS *proc, uint cpu_time) {
  std::cout << "aging" << std::endl;
  proc->reset_last_cycle(cpu_time);
  uint queue_i = m_proc_queues[proc];
  this->ages(proc);
  move_process_to(proc, queue_i-1);  
}
void SchedulerMFQS::handle_proc_tq_expire(ProcessMFQS *proc) {
  std::cout << "tq expired" << std::endl;
  uint queue_i = m_proc_queues[proc];
  move_process_to(proc, queue_i+1);
}
void SchedulerMFQS::handle_proc_stop(ProcessMFQS *proc) {
  m_tq_remaining = m_time_quantum;
  m_proc_running = false;
}
void SchedulerMFQS::handle_proc_dispose(ProcessMFQS *proc) {
  m_queues[m_proc_queues[proc]].pop_front();
  m_processes_by_age.erase(proc);
  m_proc_queues.erase(proc);
  delete proc;
}
