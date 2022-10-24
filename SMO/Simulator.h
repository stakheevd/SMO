#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <algorithm>

#include <random>
#include <time.h>

#include "buffermanager.h"
#include "consumer_manager.h"
#include "ProducerManager.h"
#include "StatisticsManager.h"


enum Mode
{
  Manual,
  Automatic
};


class Simulator
{
public:
  Simulator(int num_requests, int num_prod, int num_buf, int num_cons, double lamb);

  StepData* get_status(); // TODO: Rename??
  void take_step();
  PivotTable* get_pivot_table();
  void run_full_simulation();

private:
  double consumer_time;
  double producer_time;
  Mode mode;
  int number_requests; // TODO: RENAME
  StatisticsManager* manager;

  ConsumerManager* consumers;
  BufferManager* buffers;
  ProducerManager* producers;
};

#endif // SIMULATOR_H
