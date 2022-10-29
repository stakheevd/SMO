#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <algorithm>

#include <random>
#include <time.h>

#include "buffermanager.h"
#include "consumer_manager.h"
#include "ProducerManager.h"
#include "LoggerManager.h"


class Simulator
{
public:
  Simulator(int num_requests, int num_prod,
              int num_buf, int num_cons, double lamb);

  void take_step();
  PivotTable* get_pivot_table();
  void run_full_simulation();

  StepStatus* get_step_status();
  double get_releasing_consumer_time() const;

private:
  LoggerManager* loggers;
  ConsumerManager* consumers;
  BufferManager* buffers;
  ProducerManager* producers;

  double releasing_consumer_time;
  double creating_producer_time;
  int number_requests;
};

#endif // SIMULATOR_H
