#include "SchedulerRTS.h"

SchedulerRTS::~SchedulerRTS() {
    r_queue.clear();
  }
}

ProcessRTS *SchedulerRTS::NextProcess() {
  ProcessRTS *result = NULL;
  if(!r_queue.empty()) {
	result = r_queue.begin();
    r_queue.erase(result);
  }   
  return result;
}

void SchedulerRTS::Add(uint cpu_time, ProcessRTS *proc) {
  //update cpu_time on current running process?
  //preempt process in queue to dispatch next process
  r_queue.insert(proc);
}

uint SchedulerRTS::NextEventTime(uint cpu_time) {
  //TODO: pull remaining runtime from process at r_queue.begin();
  return 0;
}

void SchedulerRTS::DispatchEvent(uint cpu_time) {
  //TODO: Add Check for missed Deadlines, drop processes that missed, fault if in hardmode
  //TODO: Dispatch next process in queue
  return;
}
