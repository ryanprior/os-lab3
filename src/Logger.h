#ifndef LOGGER_H
#define LOGGER_H

#include "../lib/Signals/Signal.h"
#include "Process.h"
#include "Simulator.h"
#include <iostream>
using namespace Gallant;

class Logger {
public:
  Logger(std::ostream &out, SimulatorMQFS &sim)
    : m_out(out)
  {
    sim.begins.Connect(this, &Logger::begin);
    sim.ends.Connect(this, &Logger::end);
    sim.proc_arrives.Connect(this, &Logger::arrival);
  }
private:
  void listen_to(ProcessMFQS *proc);
  void listen_to(ProcessRTS *proc);
  void listen_to(ProcessWHS *proc);
  void run(Process *proc, proc_t duration);
  void exit(Process *proc);
  void tq_expire(Process *proc);
  void miss_deadline(Process *proc);
  void io(Process *proc, proc_t duration);
  void change_priority(Process *proc, proc_t new_priority);
  void change_queue(Process *proc, proc_t new_queue);
  void age_timer_expire(Process *proc);
  void begin(GenericSim *sim);
  void end(GenericSim *sim);
  void arrival(GenericSim *sim, Process *proc);
  void fault(GenericSim *sim);
  std::ostream &m_out;
};

#endif
