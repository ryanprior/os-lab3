#include "SchedulerRTS.h"

SchedulerRTS::~SchedulerRTS() {
  r_queue.clear();
}

void SchedulerRTS::Add(uint cpu_time, ProcessRTS *proc) {
  r_queue.insert(proc);
}

ProcessRTS *SchedulerRTS::NextProcess() {
  ProcessRTS *result = NULL;
  if(!r_queue.empty()) {
    result = *r_queue.begin();
  }
  return result;
}

uint SchedulerRTS::NextEventTime(uint cpu_time) const {
  uint result = 0; 
  //if queue is empty skip and return 0(will have to be handled as infinity by simulator)
  if(!m_proc_running && !r_queue.empty()){
    //if there no proc running and queue is not empty next event will be to run a proc
    result = cpu_time;
  } else{ //proc is running
    if(m_proc_running){
      //compute tick for final cycle if process makes full run;
      uint full_run = (*r_queue.begin())->time_remaining() + cpu_time;
      uint deadline = (*r_queue.begin())->deadline();
      if (full_run > deadline) {
        result = deadline;
      } else{
        result = full_run;
      }
    }
  }
  return result;
}

void SchedulerRTS::DispatchEvent(uint cpu_time) {
  //TODO: Add Check for missed Deadlines, drop processes that missed, fault if in hardmode
  //TODO: Dispatch next process in queue
  return;
}

void SchedulerRTS::AdvanceTime(uint old_time, uint new_time) {
  if (!m_proc_running){
    
  }
  //drop processes that have too much running time remaining to not make deadline
  return;
}

bool SchedulerRTS::Empty() const {
  return r_queue.empty();
}

void SchedulerRTS::handle_proc_stop(ProcessRTS *proc) {}

void SchedulerRTS::run_proc(ProcessRTS *proc, uint run_time, uint cpu_time) {}
