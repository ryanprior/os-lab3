#ifndef LOGGER_H
#define LOGGER_H

#include "Process.h"
#include "Simulator.h"
#include "SchedulerMFQS.h"
#include <Signal.h>
#include <iostream>

template <class process_T>
class Logger {
public:
  Logger(Simulator<process_T> &sim, std::ostream &out = std::cout)
    : m_out(out),
      m_sim(sim)
  {}
  virtual void Listen() {
    this->m_sim.begins.Connect(this, &Logger::begin);
    this->m_sim.ends.Connect(this, &Logger::end);
    this->m_sim.proc_arrives.Connect(this, &Logger::arrival);
  }
  virtual void listen_to (process_T *proc) {
    proc->runs.Connect(this, &Logger::run);
    proc->exits.Connect(this, &Logger::exit);
  }
  void run (process_T *proc, uint duration) {
    this->e('r', proc) << ' ' << duration << std::endl;
  }
  void exit (process_T *proc) {
    this->e('>', proc) << std::endl;
  }
  void tq_expire (process_T *proc) {
    this->e('x', proc) << std::endl;
  }
  void miss_deadline (process_T *proc) {
    this->e('!', proc) << std::endl;
  }
  void io (process_T *proc, uint duration) {
    this->e('@', proc) << ' ' << duration << std::endl;
  }
  void change_priority (process_T *proc, uint new_priority) {
    this->e('#', proc) << ' ' << new_priority << std::endl;
  }
  void change_queue (process_T *proc, uint new_queue) {
    this->e('~', proc) << ' ' << new_queue << std::endl;
  }
  void age_timer_expire (process_T *proc) {
    this->e('a', proc) << std::endl;
  }
  void begin (Simulator<process_T> *sim) {
    this->e('[') << std::endl;
  }
  void end (Simulator<process_T> *sim) {
    this->e(']') << std::endl;
  }
  void arrival (Simulator<process_T> *sim, process_T *proc) {
    this->e('<', proc) << std::endl;
  }
  void fault (Simulator<process_T> *sim) {
    this->e('F') << std::endl;
  }
protected:
  std::ostream &m_out;
  Simulator<process_T> &m_sim;
  std::ostream &e(char c, process_T *proc=NULL) {
    this->m_out << this->m_sim.cpu_time() << ' ' << c;
    if(proc) {
      this->m_out << ' ' << proc->pid();
    }
    return this->m_out;
  }
};

class LoggerMFQS : public Logger<ProcessMFQS> {
public:
  virtual void listen_to(SchedulerMFQS *scheduler) {
    scheduler->changes_queue.Connect(this, &Logger::change_queue);
    scheduler->ages.Connect(this, &Logger::age_timer_expire);
  }
  virtual void listen_to (ProcessMFQS *proc) {
    Logger::listen_to(proc);
    proc->tq_expires.Connect(this, &Logger::tq_expire); 
  }
};

#endif
