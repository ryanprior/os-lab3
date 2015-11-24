// Simulator.h: object definitions for simulators
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Process.h"
#include "Scheduler.h"
#include <Signal.h>
#include <vector>
#include <deque>
#include <iostream>


template <class process_T>
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
  Gallant::Signal1<process_T*> proc_arrives;
    // Signals with pid when the clock reaches a process's arrival
    // time and it is added to the simulation.
  const inline uint &cpu_time() const { return this->m_cpu_time; }
  void Start() {
    this->begins(this);
    for(read_proc(); m_next_arrival != NULL || !m_scheduler.empty();) {
      uint next_arrival_time = m_next_arrival->arrival();
      uint next_scheduler_event_time = m_scheduler.NextEventTime(m_cpu_time);
      if(next_arrival_time < next_scheduler_event_time) {
        uint time_difference = next_arrival_time - m_cpu_time;
        if(time_difference) {
          m_scheduler.AdvanceTime(m_cpu_time, next_arrival_time);
          m_cpu_time = next_arrival_time;
        }
        add(m_next_arrival);
        read_proc();
      } else {
        uint time_difference = next_scheduler_event_time - m_cpu_time;
        if(time_difference) {
          m_scheduler.AdvanceTime(m_cpu_time, next_scheduler_event_time);
          m_cpu_time = next_scheduler_event_time;
        }
        m_scheduler.DispatchEvent(m_cpu_time);
      }
    }
    this->ends(this);
  }
protected:
  std::istream &m_proc_stream;
  uint m_cpu_time;
  process_T *m_next_arrival;
  Scheduler<process_T> &m_scheduler;
  process_T *read_proc() {
    uint pid, bst, arr, pri, dln, io;
    if(this->m_proc_stream >> pid >> bst >> arr >> pri >> dln >> io) {
      this->m_next_arrival = new process_T(pid, bst, arr, pri, dln, io);
      return this->m_next_arrival;
    } else {
      this->m_next_arrival = NULL;
    }
    return this->m_next_arrival;
  }
  void add(process_T *proc) {
    this->proc_arrives(proc);
    this->m_scheduler.Add(m_cpu_time, proc);
  }
};

#endif
