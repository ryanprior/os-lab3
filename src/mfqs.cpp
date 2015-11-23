#include "SchedulerMFQS.h"
#include "Logger.h"

int main(int argc, char **argv) {
  uint num_queues = 3;
  uint time_q = 10;
  SchedulerMFQS scheduler(num_queues, time_q);
  Simulator<ProcessMFQS> sim(scheduler);
  Logger<ProcessMFQS> logger(sim);
  logger.Listen();
  sim.Start();
  return 0;
}
