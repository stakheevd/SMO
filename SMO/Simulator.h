#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <algorithm>

#include <random>
#include <time.h>

#include "buffermanager.h"
#include "consumer_manager.h"
#include "ProducerManager.h"
#include "StatisticsManager.h"


class Simulator
{
public:
  Simulator(int num_requests, int num_prod, int num_buf, int num_cons, double lamb);

  StepData* get_status(); // TODO: Rename??
  void take_step();
  PivotTable* get_pivot_table();
  void run_full_simulation();



  ProducerManager* getProducers() const;
  void setProducers(ProducerManager* newProducers);

  double getConsumer_time() const;
  void setConsumer_time(double newConsumer_time);

  BufferManager* getBuffers() const;
  void setBuffers(BufferManager* newBuffers);

  ConsumerManager* getConsumers() const;
  void setConsumers(ConsumerManager* newConsumers);

  StatisticsManager* getManager() const;
  void setManager(StatisticsManager* newManager);

  int getNumber_requests() const;
  void setNumber_requests(int newNumber_requests);

  double getProducer_time() const;
  void setProducer_time(double newProducer_time);

private:
  double consumer_time;
  double producer_time;
  int number_requests; // TODO: RENAME
  StatisticsManager* manager;

  ConsumerManager* consumers;
  BufferManager* buffers;
  ProducerManager* producers;
};

#endif // SIMULATOR_H
