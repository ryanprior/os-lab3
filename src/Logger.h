#ifndef LOGGER_H
#define LOGGER_H

#include "Process.h"
#include "Simulator.h"
#include <Signal.h>
#include <iostream>

template <class process_T>
class Logger {
public:
  Logger(Simulator<process_T> &sim, std::ostream &out = std::cout)
    : m_out(out),
      m_sim(sim)
  {}
  void Listen() {
    this->m_sim.begins.Connect(this, &Logger::begin);
    this->m_sim.ends.Connect(this, &Logger::end);
    this->m_sim.proc_arrives.Connect(this, &Logger::arrival);
  }
  void listen_to (process_T *proc) {
    proc->runs.Connect(this, &Logger::run);
    proc->exits.Connect(this, &Logger::exit);
  }
  void run (process_T *proc, uint duration) {
    this->e('r', proc) << ' ' << duration << std::endl;
  }
  void exit             (process_T *proc) {
    this->e('>', proc) << std::endl;
  }
  void tq_expire        (process_T *proc);
  void miss_deadline    (process_T *proc);
  void io               (process_T *proc, uint duration);
  void change_priority  (process_T *proc, uint new_priority);
  void change_queue     (process_T *proc, uint new_queue);
  void age_timer_expire (process_T *proc);
  void begin (Simulator<process_T> *sim) {
    this->e('[') << std::endl;
  }
  void end (Simulator<process_T> *sim) {
    this->e(']') << std::endl;
  }
  void arrival (Simulator<process_T> *sim, process_T *proc) {
    this->e('<', proc) << std::endl;
  }
  void fault (Simulator<process_T> *sim);
private:
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

#endif
