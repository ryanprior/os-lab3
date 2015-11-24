#include "SchedulerRTS.h"
#include "Logger.h"

int main(int argc, char **argv) {
  SchedulerRTS scheduler(false);
  Simulator<ProcessRTS> sim(scheduler);
  Logger<ProcessRTS> logger(sim);
  logger.Listen();
  sim.Start();
  return 0;
}
