#include "Simulator.h"
#include <algorithm>


/*
 * SimulatorMFQS
 */
SimulatorMFQS::~SimulatorMFQS() {
  for(auto i=this->m_queues.begin(); i != this->m_queues.end(); ++i) {
    while(!i->empty()) {
      delete i->front();
      i->pop_front();
    }
  }
}
ProcessMFQS *SimulatorMFQS::read_proc() {
  proc_t pid, bst, arr, pri;
  if(this->m_proc_stream >> pid >> bst >> arr >> pri) {
    this->m_next_arrival = new ProcessMFQS(pid, bst, arr, pri);
    return this->m_next_arrival;
  } else {
    return NULL;
  }
}
void SimulatorMFQS::add(ProcessMFQS *proc) {
  this->m_queues[0].push_back(proc);
  this->proc_arrives((GenericSim*)this, proc);
}


/*
 * SimulatorRTS
 */
ProcessRTS *SimulatorRTS::read_proc() {
  proc_t pid, bst, arr, pri, dln;
  if(this->m_proc_stream >> pid >> bst >> arr >> pri >> dln) {
    this->m_next_arrival = new ProcessRTS(pid, bst, arr, pri, dln);
    return this->m_next_arrival;
  } else {
    return NULL;
  }
}
void SimulatorRTS::add(ProcessRTS *proc) {

}


/*
 * SimulatorWHS
 */
ProcessWHS *SimulatorWHS::read_proc() {
  proc_t pid, bst, arr, pri, io;
  if(this->m_proc_stream >> pid >> bst >> arr >> pri >> io) {
    this->m_next_arrival = new ProcessWHS(pid, bst, arr, pri, io);
    return this->m_next_arrival;
  } else {
    return NULL;
  }
}
void SimulatorWHS::add(ProcessWHS *proc) {

}
