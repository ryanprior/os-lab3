// Process.h: object definitions for processes
#ifndef PROCESS_H
#define PROCESS_H

typedef unsigned int proc_t;

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
  virtual void run();
protected:
  proc_t m_pid;        // process id for external reference
  proc_t m_burst;      // total run time
  proc_t m_arrival;    // clock cycle of arrival
  proc_t m_priority;   // priority (higher priority has higher number)
  proc_t m_pc;         // program counter
};

class ProcessMFQS : public Process {
public:
  ProcessMFQS(proc_t pid,
              proc_t burst,
              proc_t arrival,
              proc_t priority)
    : Process(pid, burst, arrival, priority),
      m_last_cycle(0)
  {}
protected:
  proc_t m_last_cycle; // last cycle this process spent on the cpu
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
protected:
  proc_t m_deadline;   // last cycle this process is allowed to run
};

class ProcessWHS : public Process {
public:
  ProcessWHS(proc_t pid,
             proc_t burst,
             proc_t arrival,
             proc_t priority,
             proc_t io)
    : Process(pid, burst, arrival, priority),
      m_io(io)
  {}
protected:
  proc_t m_io;         // length of i/o in cycles (0 if no i/o)
};

#endif
