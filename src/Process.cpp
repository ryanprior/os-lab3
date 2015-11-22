#include "Process.h"
#include <sstream>


/*
 * ProcAge
 */
inline const uint& ProcAge::last_cycle() const {
  return this->m_last_cycle;
}
void ProcAge::reset_last_cycle(uint new_last_cycle) {
  this->m_last_cycle = new_last_cycle;
}


/*
 * ProcessMFQS
 */
void ProcessMFQS::Run(uint time_q, uint cpu_time) {
  uint run_time = this->m_burst - this->m_pc;
  if(run_time > time_q) {
    run_time = time_q;
  }
  this->runs(this, run_time);
  this->m_pc += run_time;
  this->reset_last_cycle(cpu_time + run_time);
  if(this->m_pc == this->m_burst) {
    this->exits(this);
  } else {
    this->tq_expires(this);
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
void ProcessRTS::Run(uint time_q, uint cpu_time) {
  uint run_time = this->m_burst - this->m_pc;
  if(run_time > time_q) {
    run_time = time_q;
  }
  if(cpu_time + run_time <= this->m_deadline) {
    this->runs(this, run_time);
    this->m_pc += run_time;
    if(this->m_pc == this->m_burst) {
      this->exits(this);
    }
  } else {
    run_time = this->m_deadline - cpu_time;
    this->runs(this, run_time);
    this->m_pc += run_time;
    this->misses_deadline(this);
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
void ProcessWHS::Run(uint time_q, uint cpu_time) {
  uint run_time = this->m_burst - this->m_pc;
  if(run_time > time_q) {
    run_time = time_q;
  }
  if(this->m_io > 0 && run_time >= time_q-1) {
    run_time = time_q - 1;
    this->runs(this, run_time);
    this->m_pc += run_time;
    this->reset_last_cycle(cpu_time + run_time);
    this->does_io(this, this->m_io);
  } else {
    this->runs(this, run_time);
    this->m_pc += run_time;
    this->reset_last_cycle(cpu_time + run_time);
    if(this->m_pc == this->m_burst) {
      this->exits(this);
    } else {
      this->tq_expires(this);
    }
  }
}
void ProcessWHS::set_priority(uint new_priority) {
  this->m_priority = new_priority;
  this->changes_priority(this, new_priority);
}
const std::string ProcessWHS::ToString() const {
  std::stringstream result;
  result << "WHF " << Process::ToString()
         << " i/o=" << this->m_io;
  return result.str();
}
