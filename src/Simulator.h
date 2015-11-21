// Simulator.h: object definitions for simulators
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Process.h"
#include "Scheduler.h"
#include <Signal.h>
#include <vector>
#include <deque>
#include <iostream>


template <typename process_T>
class Simulator {
public:
  Simulator(Scheduler<process_T> &scheduler, std::istream &proc_stream = std::cin)
    : m_proc_stream(proc_stream),
      m_cpu_time(0),
      m_next_arrival(NULL),
      m_scheduler(scheduler)
  {}
  ~Simulator() {}
  Gallant::Signal1<Simulator<process_T>*> begins;
  Gallant::Signal1<Simulator<process_T>*> ends;
  Gallant::Signal2<Simulator<process_T>*, process_T*> proc_arrives;
    // Signals with pid when the clock reaches a process's arrival
    // time and it is added to the simulation.
protected:
  std::istream &m_proc_stream;
  uint m_cpu_time;
  process_T *m_next_arrival;
  Scheduler<process_T> &m_scheduler;
  process_T *read_proc()
  {
    uint pid, bst, arr, pri, dln, io;
    if(this->m_proc_stream >> pid >> bst >> arr >> pri >> dln >> io) {
      this->m_next_arrival = new process_T(pid, bst, arr, pri, dln, io);
      return this->m_next_arrival;
    } else {
      return NULL;
    }
  }
  void add(process_T *proc)
  {
    this->proc_arrives(this, proc);
  }
  friend int main(int argc, char **argv);
};

#endif
