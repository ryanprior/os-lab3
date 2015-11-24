#include "SchedulerRTS.h"

SchedulerRTS::~SchedulerRTS() {
    while(!r_queue->empty()) {
      delete r_queue->front();
      r_queue->pop_front();
    }
  }
}

ProcessRTS *SchedulerRTS::NextProcess() {
  ProcessRTS *result = NULL;
  if(!r_queue->empty()) {
    result = r_queue->front();
    r_queue->pop_front();
  }   
  return result;
}

void SchedulerRTS::Add(uint cpu_time, ProcessRTS *proc) {
  this->r_queue.front().push_back(proc);
}

uint SchedulerRTS::NextEventTime(uint cpu_time) {
  return 0;
}

void SchedulerRTS::DispatchEvent(uint cpu_time) {
  //TODO: Add Check for missed Deadlines, drop processes that missed, fault if in hardmode
  //TODO: Dispatch next process in queue
  return;
}
