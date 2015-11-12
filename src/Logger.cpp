#include "Logger.h"

void Logger::begin(GenericSim *sim) {}
void Logger::end(GenericSim *sim) {}
void Logger::arrival(GenericSim *sim, Process *proc) {
  this->m_out << "arrived: " << proc->pid() << std::endl;
}
void Logger::fault(SimulatorRTS *sim) {}
