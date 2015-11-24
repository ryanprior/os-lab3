// Process.h: object definitions for processes
#ifndef PROCESS_H
#define PROCESS_H

#include <Signal.h>
#include <iostream>
#include <string>
#include <sstream>


template <class process_T>
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
  Gallant::Signal2<process_T*, uint> runs;  // Signals with total run
                                            // time when process runs.
  Gallant::Signal1<process_T*> exits;       // Signals on exit.
  virtual void Run(uint time_q, uint cpu_time) = 0;
  virtual inline const uint& pid() const { return this->m_pid; }
  inline const uint& arrival() const { return this->m_arrival; }
  Gallant::Signal1<process_T*> dispose;     // Signals ready for
                                            // deletion.
  friend std::ostream &operator<<(std::ostream &out, const process_T &proc) {
    out << "proc " << proc.ToString();
    return out;
  }
protected:
  virtual const std::string ToString() const {
    std::stringstream result;
    result << "pid=" << this->m_pid
           << " bst=" << this->m_burst
           << " arr=" << this->m_arrival
           << " pri=" << this->m_priority;
    return result.str();
  }
  const uint m_pid;        // process id for external reference
  const uint m_burst;      // total run time
  const uint m_arrival;    // clock cycle of arrival
        uint m_priority;   // priority (^ priority has ^ number)
        uint m_pc;         // program counter
};


class ProcAge {
public:
  virtual inline const uint& last_cycle() const {
    return this->m_last_cycle;
  }
  void reset_last_cycle(uint new_last_cycle) {
    this->m_last_cycle = new_last_cycle;
  }
protected:
  explicit ProcAge()
    : m_last_cycle(0)
  {}
  uint m_last_cycle; // Last cycle this process spent on the cpu
};


template <class process_T>
class ProcTimeQuantum {
public:
  Gallant::Signal1<process_T*> tq_expires; // Signals when the proc
                                           // runs to the end of its
                                           // given time quantum.
protected:
  explicit ProcTimeQuantum() {}
};


class ProcessMFQS : public Process<ProcessMFQS>,
                    public ProcAge,
                    public ProcTimeQuantum<ProcessMFQS> {
public:
  ProcessMFQS(uint pid,
              uint burst,
              uint arrival,
              uint priority,
              uint deadline,
              uint io)
    : Process<ProcessMFQS>(pid, burst, arrival, priority, 0, 0),
      ProcAge()
  {}
  virtual ~ProcessMFQS() {}
  virtual void Run(uint time_q, uint cpu_time);
  struct compare_by_age {
    bool operator() (const ProcessMFQS *lhs, const ProcessMFQS *rhs) const {
      bool result;
      if(lhs->last_cycle() < rhs->last_cycle()) {
        result = true;
      } else {
        if(lhs->last_cycle() == rhs->last_cycle()) {
          result = lhs->pid() < rhs->pid();
        } else {
          result = false;
        }
      }
      return result;
    }
  };
protected:
  virtual const std::string ToString() const;
};


class ProcessRTS : public Process<ProcessRTS> {
public:
  ProcessRTS(uint pid,
             uint burst,
             uint arrival,
             uint priority,
             uint deadline,
             uint io)
    : Process<ProcessRTS>(pid, burst, arrival, priority, 0, 0),
      m_deadline(deadline)
  {}
  virtual ~ProcessRTS() {}
  Gallant::Signal1<ProcessRTS*> misses_deadline; // Signals when process
                                              // misses its deadline.
  virtual void Run(uint time_q, uint cpu_time);
protected:
  virtual const std::string ToString() const;
  const uint m_deadline;   // last cycle this process is allowed to run
};


class ProcessWHS : public Process<ProcessWHS>,
                   public ProcAge,
                   public ProcTimeQuantum<ProcessWHS> {
public:
  ProcessWHS(uint pid,
             uint burst,
             uint arrival,
             uint priority,
             uint deadline,
             uint io)
    : Process<ProcessWHS>(pid, burst, arrival, priority, 0, 0),
      ProcAge(),
      m_io(io)
  {}
  virtual ~ProcessWHS() {}
  Gallant::Signal2<ProcessWHS*, uint>
  does_io; // Signals with duration when the process begins doing I/O.
  Gallant::Signal2<ProcessWHS*, uint>
  changes_priority; // Signals with new priority.
  virtual void Run(uint time_q, uint cpu_time);
  void set_priority(uint new_priority);
protected:
  virtual const std::string ToString() const;
  const uint m_io;         // length of i/o in cycles (0 if no i/o)
};

#endif
