#include "SchedulerMFQS.h"
#include "Logger.h"

int main(int argc, char **argv) {
  uint num_queues = 3;
  uint time_q = 10;
  uint age_time = 14;
  SchedulerMFQS scheduler(num_queues, time_q, age_time);
  Simulator<ProcessMFQS> sim(scheduler);
  LoggerMFQS logger(sim);
  logger.Listen();
  sim.Start();
  return 0;
}
