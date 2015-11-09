#include "Process.h"
#include <sstream>

std::ostream &operator<<(std::ostream &out, const Process &proc) {
  out << "proc " << proc.ToString();
  return out;
}

std::string Process::ToString() const {
  std::stringstream result;
  result << "pid=" << this->m_pid
         << " bst=" << this->m_burst
         << " arr=" << this->m_arrival
         << " pri=" << this->m_priority;
  return result.str();
}

std::string ProcessMFQS::ToString() const {
  std::stringstream result;
  result << "MFQS " << Process::ToString()
         << " lst=" << this->m_last_cycle;
  return result.str();
}
