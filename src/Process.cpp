#include "Process.h"
#include <sstream>

std::ostream &operator<<(std::ostream &out, const Process &proc) {
  out << "proc " << proc.ToString();
  return out;
}


/*
 * Process
 */
const std::string Process::ToString() const {
  std::stringstream result;
  result << "pid=" << this->m_pid
         << " bst=" << this->m_burst
         << " arr=" << this->m_arrival
         << " pri=" << this->m_priority;
  return result.str();
}
inline const proc_t& Process::pid() const {
  return this->m_pid;
}
inline const proc_t& Process::arrival() const {
  return this->m_arrival;
}


/*
 * ProcAge
 */
inline const proc_t& ProcAge::last_cycle() const {
  return this->m_last_cycle;
}
void ProcAge::reset_last_cycle(proc_t new_last_cycle) {
  this->m_last_cycle = new_last_cycle;
}


/*
 * ProcessMFQS
 */
void ProcessMFQS::Run(proc_t time_q, proc_t cpu_time) {
  proc_t run_time = this->m_burst - this->m_pc;
  if(run_time > time_q) {
    run_time = time_q;
  }
  this->proc_run(run_time);
  this->m_pc += run_time;
  this->reset_last_cycle(cpu_time + run_time);
  if(this->m_pc == this->m_burst) {
    this->proc_exit();
  } else {
    this->proc_tq_expired();
  }
}
const std::string ProcessMFQS::ToString() const {
  std::stringstream result;
  result << "MFQS " << Process::ToString()
         << " lst=" << this->m_last_cycle;
  return result.str();
}


/*
 * ProcessRTS
 */
void ProcessRTS::Run(proc_t time_q, proc_t cpu_time) {
  proc_t run_time = this->m_burst - this->m_pc;
  if(run_time > time_q) {
    run_time = time_q;
  }
  if(cpu_time + run_time <= this->m_deadline) {
    this->proc_run(run_time);
    this->m_pc += run_time;
    if(this->m_pc == this->m_burst) {
      this->proc_exit();
    }
  } else {
    run_time = this->m_deadline - cpu_time;
    this->proc_run(run_time);
    this->m_pc += run_time;
    this->proc_missed_deadline();
  }
}
const std::string ProcessRTS::ToString() const {
  std::stringstream result;
  result << "RTS " << Process::ToString()
         << " dln=" << this->m_deadline;
  return result.str();
}


/*
 * ProcessWHS
 */
void ProcessWHS::Run(proc_t time_q, proc_t cpu_time) {
  proc_t run_time = this->m_burst - this->m_pc;
  if(run_time > time_q) {
    run_time = time_q;
  }
  if(this->m_io > 0 && run_time >= time_q-1) {
    run_time = time_q - 1;
    this->proc_run(run_time);
    this->m_pc += run_time;
    this->reset_last_cycle(cpu_time + run_time);
    this->proc_does_io(this->m_io);
  } else {
    this->proc_run(run_time);
    this->m_pc += run_time;
    this->reset_last_cycle(cpu_time + run_time);
    if(this->m_pc == this->m_burst) {
      this->proc_exit();
    } else {
      this->proc_tq_expired();
    }
  }
}
void ProcessWHS::set_priority(proc_t new_priority) {
  this->m_priority = new_priority;
  this->proc_changes_priority(new_priority);
}
const std::string ProcessWHS::ToString() const {
  std::stringstream result;
  result << "WHF " << Process::ToString()
         << " i/o=" << this->m_io;
  return result.str();
}
