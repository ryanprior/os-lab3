// Simulator.h: object definitions for simulators
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Process.h"
#include <Signal.h>
#include <vector>
#include <deque>
#include <iostream>

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
  Gallant::Signal1<GenericSim*> begins;
  Gallant::Signal1<GenericSim*> ends;
  Gallant::Signal2<GenericSim*, Process*> proc_arrives;
    // Signals with pid when the clock reaches a process's arrival
    // time and it is added to the simulation.
protected:
  std::istream &m_proc_stream;
  uint m_cpu_time;
  T *m_next_arrival;
  virtual T *read_proc() = 0;
  virtual void add(T *proc) = 0;
  friend int main(int argc, char **argv);
};


class SimTimeQuantum {
public:
  SimTimeQuantum(uint time_q)
    : m_time_quantum(time_q)
  {}
protected:
  uint m_time_quantum;
};


class SimulatorMFQS : public Simulator<ProcessMFQS>,
                      SimTimeQuantum {
public:
  SimulatorMFQS(uint num_queues,
                uint time_q,
                std::istream &proc_stream = std::cin)
    : Simulator(proc_stream),
      SimTimeQuantum(time_q),
      m_queues(num_queues)
  {}
  virtual ~SimulatorMFQS();
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
  Gallant::Signal1<SimulatorRTS*> faults; // Signals when a process
                                          // cannot meet its deadline
                                          // during hard RT operation.
protected:
  virtual ProcessRTS *read_proc();
  virtual void add(ProcessRTS *proc);
  const Type m_type;
};


class SimulatorWHS : public Simulator<ProcessWHS>, SimTimeQuantum {
public:
  SimulatorWHS(uint time_q, std::istream &proc_stream = std::cin)
    : Simulator(proc_stream),
      SimTimeQuantum(time_q)
  {}
  virtual ~SimulatorWHS() {}
protected:
  virtual ProcessWHS *read_proc();
  virtual void add(ProcessWHS *proc);
};

#endif
