// Process.h: object definitions for processes
#ifndef PROCESS_H
#define PROCESS_H

#include <Signal.h>
#include <iostream>
#include <string>


class Process {
public:
  explicit Process(uint pid,
                   uint burst,
                   uint arrival,
                   uint priority,
                   uint deadline,
                   uint io)
    : m_pid(pid),
      m_burst(burst),
      m_arrival(arrival),
      m_priority(priority),
      m_pc(0)
  {}
  virtual ~Process() {}
  Gallant::Signal2<Process*, uint> runs;  // Signals with total run
                                          // time when process runs.
  Gallant::Signal1<Process*> exits;       // Signals on exit.
  virtual void Run(uint time_q, uint cpu_time) = 0;
  inline const uint& pid() const { return this->m_pid; }
  inline const uint& arrival() const { return this->m_arrival; }
  friend std::ostream &operator<<(std::ostream &out, const Process &proc);
protected:
  virtual const std::string ToString() const;
  const uint m_pid;        // process id for external reference
  const uint m_burst;      // total run time
  const uint m_arrival;    // clock cycle of arrival
        uint m_priority;   // priority (^ priority has ^ number)
        uint m_pc;         // program counter
};


class ProcAge {
public:
  explicit ProcAge()
    : m_last_cycle(0)
  {}
  inline const uint& last_cycle() const;
  void reset_last_cycle(uint new_last_cycle);
protected:
  uint m_last_cycle; // Last cycle this process spent on the cpu
};


class ProcTimeQuantum {
public:
  Gallant::Signal1<Process*> tq_expires; // Signals when the proc runs
                                         // to the end of its given
                                         // time quantum.
};


class ProcessMFQS : public Process, ProcAge, ProcTimeQuantum {
public:
  ProcessMFQS(uint pid,
              uint burst,
              uint arrival,
              uint priority,
              uint deadline,
              uint io)
    : Process(pid, burst, arrival, priority, 0, 0),
      ProcAge()
  {}
  virtual ~ProcessMFQS() {}
  virtual void Run(uint time_q, uint cpu_time);
protected:
  virtual const std::string ToString() const;
};


class ProcessRTS : public Process {
public:
  ProcessRTS(uint pid,
             uint burst,
             uint arrival,
             uint priority,
             uint deadline,
             uint io)
    : Process(pid, burst, arrival, priority, 0, 0),
      m_deadline(deadline)
  {}
  virtual ~ProcessRTS() {}
  Gallant::Signal1<Process*> misses_deadline; // Signals when process
                                              // misses its deadline.
  virtual void Run(uint time_q, uint cpu_time);
protected:
  virtual const std::string ToString() const;
  const uint m_deadline;   // last cycle this process is allowed to run
};


class ProcessWHS : public Process, ProcAge, ProcTimeQuantum {
public:
  ProcessWHS(uint pid,
             uint burst,
             uint arrival,
             uint priority,
             uint deadline,
             uint io)
    : Process(pid, burst, arrival, priority, 0, 0),
      ProcAge(),
      m_io(io)
  {}
  virtual ~ProcessWHS() {}
  Gallant::Signal2<Process*, uint>
  does_io; // Signals with duration when the process begins doing I/O.
  Gallant::Signal2<Process*, uint>
  changes_priority; // Signals with new priority.
  virtual void Run(uint time_q, uint cpu_time);
  void set_priority(uint new_priority);
protected:
  virtual const std::string ToString() const;
  const uint m_io;         // length of i/o in cycles (0 if no i/o)
};

#endif
