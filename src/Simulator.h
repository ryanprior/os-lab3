#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "../lib/Signals/Signal.h"
#include "Process.h"
#include <vector>
#include <iostream>
using namespace Gallant;


template <typename T>
class Simulator {
public:
  Simulator(std::istream &proc_stream)
    : m_proc_stream(proc_stream),
      m_cpu_time(0),
      m_next_arrival(NULL)
  {}
  virtual ~Simulator() {}
  Signal0<> sim_begins; // Signals at the beginning of a simulation.
  Signal0<> sim_ends;   // Signals at the end of a simulation.
  Signal1<proc_t> sim_proc_arrives;  // Signals with pid when the
                                     // clock reaches a process's
                                     // arrival time and it is added
                                     // to the simulation.
protected:
  std::istream &m_proc_stream;
  proc_t m_cpu_time;
  virtual T *read_proc() = 0;
  T *m_next_arrival;
};


class SimTimeQuantum {
public:
  SimTimeQuantum(proc_t time_q)
    : m_time_quantum(time_q)
  {}
protected:
  proc_t m_time_quantum;
};


class SimulatorMQFS : public Simulator<ProcessMFQS>, SimTimeQuantum {
public:
  SimulatorMQFS(std::istream &proc_stream, proc_t num_queues, proc_t time_q)
    : Simulator(proc_stream),
      SimTimeQuantum(time_q)
  {} // TODO set up queue structures
  virtual ~SimulatorMQFS() {}
protected:
  virtual ProcessMFQS *read_proc();
};


class SimulatorRTS : public Simulator<ProcessRTS> {
public:
  enum Type { HARD, SOFT };
  SimulatorRTS(std::istream &proc_stream, Type type)
    : Simulator(proc_stream),
      m_type(type)
  {}
  virtual ~SimulatorRTS() {}
  Signal0<> sim_faults; // Signals when a process cannot meet its
                        // deadline during hard real-time operation.
protected:
  virtual ProcessRTS *read_proc();
  const Type m_type;
};


class SimulatorWHS : public Simulator<ProcessWHS>, SimTimeQuantum {
public:
  SimulatorWHS(std::istream &proc_stream, proc_t time_q)
    : Simulator(proc_stream),
      SimTimeQuantum(time_q)
  {}
  virtual ~SimulatorWHS() {}
protected:
  virtual ProcessWHS *read_proc();
};

#endif
