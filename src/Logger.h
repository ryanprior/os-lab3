#ifndef LOGGER_H
#define LOGGER_H

#include <Signal.h>
#include "Process.h"
#include "Simulator.h"
#include <iostream>
using namespace Gallant;

class Logger {
public:
  Logger(SimulatorMFQS *sim, std::ostream &out = std::cout)
    : m_out(out)
  {
    this->listen_to((GenericSim*)sim);
  }
  Logger(SimulatorRTS *sim, std::ostream &out = std::cout)
    : m_out(out)
  {
    this->listen_to((GenericSim*)sim);
    sim->faults.Connect(this, &Logger::fault);
  }
  Logger(SimulatorWHS *sim, std::ostream &out = std::cout)
    : m_out(out)
  {
    this->listen_to((GenericSim*)sim);
  }
private:
  void listen_to(GenericSim *sim)
  {
    sim->begins.Connect(this, &Logger::begin);
    sim->ends.Connect(this, &Logger::end);
    sim->proc_arrives.Connect(this, &Logger::arrival);
  }
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
  void fault(SimulatorRTS *sim);
  std::ostream &m_out;
};

#endif
