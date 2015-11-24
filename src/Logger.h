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
  explicit Logger(Simulator<process_T> &sim, std::ostream &out = std::cout)
    : m_out(out),
      m_sim(sim)
  {}
  virtual void Listen() {
    this->m_sim.begins.Connect(this, &Logger::begin);
    this->m_sim.ends.Connect(this, &Logger::end);
    this->m_sim.proc_arrives.Connect(this, &Logger::arrival);
    this->m_sim.proc_arrives.Connect(this, &Logger::listen_to);
    this->m_sim.begins.Connect(this, &Logger::listen_to);

  }
  virtual void listen_to (process_T *proc) {
    proc->runs.Connect(this, &Logger::run);
    proc->exits.Connect(this, &Logger::exit);
  }
  virtual void listen_to (Scheduler<process_T> *scheduler) {
    scheduler->changes_queue.Connect(this, &Logger::change_queue);
    scheduler->ages.Connect(this, &Logger::age_timer_expire);
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
  void begin (Scheduler<process_T> *sim) {
    this->e('[') << std::endl;
  }
  void end (Scheduler<process_T> *sim) {
    this->e(']') << std::endl;
  }
  void arrival (process_T *proc) {
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
  LoggerMFQS(Simulator<ProcessMFQS> &sim)
    : Logger<ProcessMFQS>(sim)
  {}
  virtual void listen_to(SchedulerMFQS *scheduler) {
    scheduler->changes_queue.Connect(this, &Logger<ProcessMFQS>::change_queue);
    scheduler->ages.Connect(this, &Logger<ProcessMFQS>::age_timer_expire);
  }
  virtual void listen_to (ProcessMFQS *proc) {
    Logger<ProcessMFQS>::listen_to(proc);
    proc->tq_expires.Connect(this, &Logger<ProcessMFQS>::tq_expire);
  }
};

#endif
