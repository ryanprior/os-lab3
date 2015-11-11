#include "Process.h"
#include "Simulator.h"
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
    if(type == "mfqs") {
      sim = (Simulator<Process>*)new SimulatorMQFS(cin, 0, 0);
    } else if(type == "rts") {
      sim = (Simulator<Process>*)new SimulatorRTS(cin, SimulatorRTS::HARD);
    } else if(type == "whs") {
      sim = (Simulator<Process>*)new SimulatorWHS(cin, 0);
    } else {
      cout << "unknown type: " << type << endl;
      exit(1);
    }
    while(sim->read_proc()) {
      cout << *sim->m_next_arrival << endl;
      delete sim->m_next_arrival;
    }
    delete sim;
  }
  return 0;
}
