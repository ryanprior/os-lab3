#include "Process.h"
#include "Simulator.h"
#include "SchedulerMFQS.h"
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
    if(type == "mfqs") {
      uint num_queues = 3;
      uint time_q = 10;
      SchedulerMFQS *scheduler = new SchedulerMFQS(num_queues, time_q);
      Simulator<ProcessMFQS> sim(*scheduler);
      Logger<ProcessMFQS> logger(sim);
      logger.Listen();
      sim.Start();
      delete scheduler;
    } else if(type == "rts") {
      cout << "rts unimplemented ¯\\_(ツ)_/¯" << endl;
      exit(1);
    } else if(type == "whs") {
      cout << "whs unimplemented ¯\\_(ツ)_/¯" << endl;
      exit(1);
    } else {
      cout << "unknown type: " << type << endl;
      exit(1);
    }
  }
  return 0;
}
