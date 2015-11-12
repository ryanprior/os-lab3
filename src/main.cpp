#include "Process.h"
#include "Simulator.h"
#include "Logger.h"
#include <iostream>
#include <algorithm>

int main(int argc, char **argv) {
  using namespace std;
  if(argc != 2) {
    cout << "wrong number of arguments" << endl;
    exit(1);
  } else {
    // Get lower case first argument as string
    std::string type(argv[1]);
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    // Read in records and print their representations to stdout
    Simulator<Process> *sim;
    Logger *logger;
    if(type == "mfqs") {
      auto s = new SimulatorMFQS(3, 10);
      logger = new Logger(s);
      sim = (Simulator<Process>*) s;
    } else if(type == "rts") {
      auto s = new SimulatorRTS(SimulatorRTS::HARD);
      logger = new Logger(s);
      sim = (Simulator<Process>*) s;
    } else if(type == "whs") {
      auto s = new SimulatorWHS(20);
      logger = new Logger(s);
      sim = (Simulator<Process>*) s;
    } else {
      cout << "unknown type: " << type << endl;
      exit(1);
    }
    while(sim->read_proc()) {
      cout << "read: " << *sim->m_next_arrival << endl;
      sim->add(sim->m_next_arrival);
    }
    delete sim;
    delete logger;
  }
  return 0;
}
