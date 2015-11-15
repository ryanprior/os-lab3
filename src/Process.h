// Process.h: object definitions for processes
#ifndef PROCESS_H
#define PROCESS_H

#include <Signal.h>
#include <iostream>
#include <string>
typedef unsigned int proc_t;
using namespace Gallant;


class Process {
public:
  explicit Process(proc_t pid,
                   proc_t burst,
                   proc_t arrival,
                   proc_t priority)
    : m_pid(pid),
      m_burst(burst),
      m_arrival(arrival),
      m_priority(priority),
      m_pc(0)
  {}
  virtual ~Process() {}
  Signal2<Process*, proc_t> runs;  // Signals with total run time when
                                   // the process runs.
  Signal1<Process*> exits;         // Signals on exit.
  virtual void Run(proc_t time_q, proc_t cpu_time) = 0;
  inline const proc_t& pid() const { return this->m_pid; }
  inline const proc_t& arrival() const { return this->m_arrival; }
  friend std::ostream &operator<<(std::ostream &out, const Process &proc);
protected:
  virtual const std::string ToString() const;
  const proc_t m_pid;        // process id for external reference
  const proc_t m_burst;      // total run time
  const proc_t m_arrival;    // clock cycle of arrival
        proc_t m_priority;   // priority (^ priority has ^ number)
        proc_t m_pc;         // program counter
};


class ProcAge {
public:
  explicit ProcAge()
    : m_last_cycle(0)
  {}
  inline const proc_t& last_cycle() const;
  void reset_last_cycle(proc_t new_last_cycle);
protected:
  proc_t m_last_cycle; // Last cycle this process spent on the cpu
};


class ProcTimeQuantum {
public:
  Signal1<Process*> tq_expires; // Signals when the proc runs to the
                                // end of its given time quantum.
};


class ProcessMFQS : public Process, ProcAge, ProcTimeQuantum {
public:
  ProcessMFQS(proc_t pid,
              proc_t burst,
              proc_t arrival,
              proc_t priority)
    : Process(pid, burst, arrival, priority),
      ProcAge()
  {}
  virtual ~ProcessMFQS() {}
  virtual void Run(proc_t time_q, proc_t cpu_time);
protected:
  virtual const std::string ToString() const;
};


class ProcessRTS : public Process {
public:
  ProcessRTS(proc_t pid,
             proc_t burst,
             proc_t arrival,
             proc_t priority,
             proc_t deadline)
    : Process(pid, burst, arrival, priority),
      m_deadline(deadline)
  {}
  virtual ~ProcessRTS() {}
  Signal1<Process*> misses_deadline; // Signals when process misses
                                     // its deadline.
  virtual void Run(proc_t time_q, proc_t cpu_time);
protected:
  virtual const std::string ToString() const;
  const proc_t m_deadline;   // last cycle this process is allowed to run
};


class ProcessWHS : public Process, ProcAge, ProcTimeQuantum {
public:
  ProcessWHS(proc_t pid,
             proc_t burst,
             proc_t arrival,
             proc_t priority,
             proc_t io)
    : Process(pid, burst, arrival, priority),
      ProcAge(),
      m_io(io)
  {}
  virtual ~ProcessWHS() {}
  Signal2<Process*, proc_t> does_io; // Signals with duration when the
                                     // process begins doing I/O.
  Signal2<Process*, proc_t> changes_priority; // Signals with new
                                              // priority.
  virtual void Run(proc_t time_q, proc_t cpu_time);
  void set_priority(proc_t new_priority);
protected:
  virtual const std::string ToString() const;
  const proc_t m_io;         // length of i/o in cycles (0 if no i/o)
};

#endif
