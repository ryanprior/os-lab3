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
  bool was_empty = Empty();
  proc->reset_last_cycle(cpu_time);
  m_queues.front().push_back(proc);
  m_proc_queues[proc] = 0;
  proc->dispose.Connect(this, &SchedulerMFQS::handle_proc_dispose);
  proc->tq_expires.Connect(this, &SchedulerMFQS::handle_proc_tq_expire);
  proc->tq_expires.Connect(this, &SchedulerMFQS::handle_proc_stop);
  proc->exits.Connect(this, &SchedulerMFQS::handle_proc_stop);
  if(was_empty && !m_proc_just_stopped) {
    m_tq_remaining = m_time_quantum;
    run_proc(proc, 1, cpu_time);
  }
}

uint SchedulerMFQS::NextEventTime(uint cpu_time) const {
  const int max = std::numeric_limits<uint>::max();
  uint result = max;
  int i = first_non_empty_queue_index();
  if(i >= 0 && !m_proc_running && !m_proc_just_stopped) {
    result = 0; // need to let a process start
  }
  if(m_proc_running && i>=0) {
    result = m_queues[i].front()->time_remaining(cpu_time);
    if(i < m_queues.size()) {
      result = std::min(result, m_tq_remaining);
    }
  }
  auto oldest = oldest_proc();
  if(oldest) {
    result = std::min(result, m_age_time + oldest->last_cycle() - cpu_time);
  }
  if(!m_proc_running && !Empty()) {
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
    if(!m_proc_running && !m_proc_just_stopped) {
      if(m_proc_queues[proc] < m_queues.size()-1) {
        m_tq_remaining = time_q_for(m_proc_queues[proc]);
      } else {
        m_fcfs = true;
      }
      run_proc(proc, 0, cpu_time);
    } else {
      ProcessMFQS *oldest = oldest_proc();
      if(oldest
         && m_age_time + oldest->last_cycle() == cpu_time) {
        age_process(oldest, cpu_time);
      }
    }
  }
}

void SchedulerMFQS::AdvanceTime(uint old_time, uint new_time) {
  m_proc_just_stopped = false;
  if(m_proc_running) {
    ProcessMFQS *proc = m_queues[first_non_empty_queue_index()].front();
    uint run_time = new_time - old_time;
    run_proc(proc, run_time, new_time);
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
  return *m_processes_by_age.begin();
}

void SchedulerMFQS::move_process_to(ProcessMFQS *proc, uint new_queue) {
  uint queue_i = m_proc_queues[proc];
  m_queues[queue_i].pop_front();
  m_proc_queues[proc] = new_queue;
  m_queues[new_queue].push_back(proc);
  if(new_queue > 1) {
    m_processes_by_age.insert(proc);
  }
  this->changes_queue(proc, new_queue);
}


void SchedulerMFQS::age_process(ProcessMFQS *proc, uint cpu_time) {
  m_processes_by_age.erase(proc);
  proc->reset_last_cycle(cpu_time);
  uint queue_i = m_proc_queues[proc];
  move_process_to(proc, queue_i-1);
  this->ages(proc);
}
void SchedulerMFQS::handle_proc_tq_expire(ProcessMFQS *proc) {
  uint queue_i = m_proc_queues[proc];
  move_process_to(proc, queue_i+1);
}
void SchedulerMFQS::handle_proc_stop(ProcessMFQS *proc) {
  m_proc_running = false;
  m_proc_just_stopped = true;
  if(m_proc_queues[proc] > 1) {
    m_processes_by_age.insert(proc);
  }
}
void SchedulerMFQS::handle_proc_dispose(ProcessMFQS *proc) {
  m_queues[m_proc_queues[proc]].pop_front();
  m_processes_by_age.erase(proc);
  m_proc_queues.erase(proc);
  delete proc;
}
void SchedulerMFQS::run_proc(ProcessMFQS *proc, uint run_time, uint cpu_time) {
  m_proc_running = true;
  m_processes_by_age.erase(proc);
  if(m_fcfs) {
    proc->Run(run_time, run_time, cpu_time);
    m_tq_remaining = 0;
  } else {
    proc->Run(run_time, m_tq_remaining, cpu_time);
    m_tq_remaining -= run_time;
  }
}
