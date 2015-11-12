#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "../lib/Signals/Signal.h"
#include "Process.h"
#include <vector>
#include <deque>
#include <iostream>
using namespace Gallant;

template <typename T> class Simulator;
typedef Simulator<Process> GenericSim;


template <typename T>
class Simulator {
public:
  Simulator(std::istream &proc_stream = std::cin)
    : m_proc_stream(proc_stream),
      m_cpu_time(0),
      m_next_arrival(NULL)
  {}
  virtual ~Simulator() {}
  Signal1<GenericSim*> begins; // Signals at the beginning of a
                               // simulation.
  Signal1<GenericSim*> ends;   // Signals at the end of a simulation.
  Signal2<GenericSim*, Process*> proc_arrives;
     // Signals with pid when the clock reaches a process's arrival
     // time and it is added to the simulation.
protected:
  std::istream &m_proc_stream;
  proc_t m_cpu_time;
  T *m_next_arrival;
  virtual T *read_proc() = 0;
  virtual void add(T *proc) = 0;
  friend int main(int argc, char **argv);
};


class SimTimeQuantum {
public:
  SimTimeQuantum(proc_t time_q)
    : m_time_quantum(time_q)
  {}
protected:
  proc_t m_time_quantum;
};


class SimulatorMQFS : public Simulator<ProcessMFQS>,
                      SimTimeQuantum {
public:
  SimulatorMQFS(proc_t num_queues,
                proc_t time_q,
                std::istream &proc_stream = std::cin)
    : Simulator(proc_stream),
      SimTimeQuantum(time_q),
      m_queues(num_queues)
  {}
  virtual ~SimulatorMQFS() {}
protected:
  virtual ProcessMFQS *read_proc();
  virtual void add(ProcessMFQS *proc);
  std::vector<std::deque<ProcessMFQS*> > m_queues;
};


class SimulatorRTS : public Simulator<ProcessRTS> {
public:
  enum Type { HARD, SOFT };
  SimulatorRTS(Type type, std::istream &proc_stream = std::cin)
    : Simulator(proc_stream),
      m_type(type)
  {}
  virtual ~SimulatorRTS() {}
  Signal1<SimulatorRTS*> faults; // Signals when a process cannot meet
                                 // its deadline during hard real-time
                                 // operation.
protected:
  virtual ProcessRTS *read_proc();
  virtual void add(ProcessRTS *proc);
  const Type m_type;
};


class SimulatorWHS : public Simulator<ProcessWHS>, SimTimeQuantum {
public:
  SimulatorWHS(proc_t time_q, std::istream &proc_stream = std::cin)
    : Simulator(proc_stream),
      SimTimeQuantum(time_q)
  {}
  virtual ~SimulatorWHS() {}
protected:
  virtual ProcessWHS *read_proc();
  virtual void add(ProcessWHS *proc);
};

#endif
