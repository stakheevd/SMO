#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <algorithm>
#include <random>

#include "BufferManager.h"
#include "ConsumerManager.h"
#include "ProducerManager.h"
#include "LoggerManager.h"

class Simulator
{
public:
  Simulator(int num_requests, int num_prod,
              int num_buf, int num_cons, double lamb);

  void run_full_simulation();
  void take_step();

  PivotTable* get_pivot_table();
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
