#ifndef LOGGER_H
#define LOGGER_H

#include "Process.h"
#include "Simulator.h"
#include <Signal.h>
#include <iostream>

template <typename proc_T>
class Logger {
public:
  Logger(std::ostream &out = std::cout)
    : m_out(out)
  {}
  void listen_to(Simulator<proc_T> &sim)
  {
    sim.begins.Connect(this, &Logger::begin);
    sim.ends.Connect(this, &Logger::end);
    sim.proc_arrives.Connect(this, &Logger::arrival);
  }
  void listen_to        (proc_T *proc);
  void run              (proc_T *proc, uint duration);
  void exit             (proc_T *proc);
  void tq_expire        (proc_T *proc);
  void miss_deadline    (proc_T *proc);
  void io               (proc_T *proc, uint duration);
  void change_priority  (proc_T *proc, uint new_priority);
  void change_queue     (proc_T *proc, uint new_queue);
  void age_timer_expire (proc_T *proc);
  void begin            (Simulator<proc_T> *sim) {}
  void end              (Simulator<proc_T> *sim) {}
  void arrival          (Simulator<proc_T> *sim, proc_T *proc)
  {
    this->m_out << "arrived: " << proc->pid() << std::endl;
  }
  void fault            (Simulator<proc_T> *sim);
  std::ostream &m_out;
};

#endif
