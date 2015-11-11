#include "Simulator.h"


/*
 * SimulatorMQFS
 */
ProcessMFQS *SimulatorMQFS::read_proc() {
  proc_t pid, bst, arr, pri;
  if(this->m_proc_stream >> pid >> bst >> arr >> pri) {
    this->m_next_arrival = new ProcessMFQS(pid, bst, arr, pri);
    return this->m_next_arrival;
  } else {
    return NULL;
  }
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
