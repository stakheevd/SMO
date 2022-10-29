#ifndef PRODUCERMANAGER_H
#define PRODUCERMANAGER_H

#include "request.h"
#include "producer.h"
#include "StatisticsManager.h"

class ProducerManager
{
public:
  ProducerManager(StatisticsManager* manager, int num_prod, double lamb);

  Request* submit_request();

  double get_creating_producer_time() const;
  int get_number_released_requests() const;
  const std::vector<Producer>& get_producers() const;
  std::vector<Request*> get_all_requests() const;

private:
  std::vector<Producer> producers;
  StatisticsManager* st_manager;
  int creating_producer_id;
  int number_released_requests;
  double creating_producer_time;

  void select_creating_producer();
};

#endif // PRODUCERMANAGER_H
