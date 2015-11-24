#include "Process.h"
#include <algorithm>


/*
 * ProcessMFQS
 */
void ProcessMFQS::Run(uint run_time, uint time_q, uint cpu_time) {
  uint time_remaining = m_burst - m_pc;
  run_time = std::min({run_time, time_remaining, time_q});
  if(!m_running) {
    m_running = true;
    uint total_run_time = std::min(m_burst - m_pc, time_q);
    this->runs(this, total_run_time);
  }
  m_pc += run_time;
  this->reset_last_cycle(cpu_time);
  if(m_pc == m_burst) {
    this->exits(this);
    m_running = false;
    this->dispose(this);
  } else if(run_time == time_q) {
    this->tq_expires(this);
    m_running = false;
  }
}
const std::string ProcessMFQS::ToString() const {
  std::stringstream result;
  result << "MFQS " << Process<ProcessMFQS>::ToString()
         << " lst=" << this->m_last_cycle;
  return result.str();
}


/*
 * ProcessRTS
 */
void ProcessRTS::Run(uint run_time, uint time_q, uint cpu_time) {
  run_time = std::min({run_time, m_burst - m_pc, time_q});
  this->runs(this, run_time);
  this->m_pc += run_time;
  if(cpu_time > m_deadline) {
    this->misses_deadline(this);
    this->dispose(this);
  } else if(m_pc == m_burst) {
    this->exits(this);
    this->dispose(this);
  }
}

const std::string ProcessRTS::ToString() const {
  std::stringstream result;
  result << "RTS " << Process<ProcessRTS>::ToString()
         << " dln=" << this->m_deadline;
  return result.str();
}


/*
 * ProcessWHS
 */
void ProcessWHS::Run(uint run_time, uint time_q, uint cpu_time) {
  run_time = std::min({run_time, m_burst - m_pc, time_q});
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
      this->dispose(this);
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
  result << "WHF " << Process<ProcessWHS>::ToString()
         << " i/o=" << this->m_io;
  return result.str();
}
