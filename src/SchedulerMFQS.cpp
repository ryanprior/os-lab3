#include "SchedulerMFQS.h"

SchedulerMFQS::~SchedulerMFQS() {
  for(auto i=this->m_queues.begin(); i != this->m_queues.end(); ++i) {
    while(!i->empty()) {
      delete i->front();
      i->pop_front();
    }
  }
}

ProcessMFQS *SchedulerMFQS::NextProcess() {
  ProcessMFQS *result = NULL;
  for(auto i=this->m_queues.begin();
      !result && i != this->m_queues.end();
      ++i)
    {
      if(!i->empty()) {
        result = i->front();
        i->pop_front();
      }
    }
  return result;
}
