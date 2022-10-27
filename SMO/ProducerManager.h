#ifndef PRODUCERMANAGER_H
#define PRODUCERMANAGER_H

#include "request.h"
#include "producer.h"
#include "StatisticsManager.h"

class ProducerManager
{
public:
  ProducerManager(StatisticsManager* manager, int num_prod, double lamb);

  //void insert_request(Request req);
//  Request emplace_request(Request req);

	std::vector<Request*> get_all_requests();
	Request* submit_request(); //nextEmit

  double get_next_event_time() const;
  void set_next_event_time(double new_next_event_time);

  int get_released_requests() const;
  void set_released_requests(int new_released_requests);

  const std::vector<Producer>& getProducers() const;

private:
  std::vector<Producer> producers; // TODO: MB pointer?
  StatisticsManager* st_manager;
  int producer_id; //indexOfSourceToEmit
  int released_requests;
  double next_event_time;

  void init_next_event_time(); //init_next_submit_time
};

#endif // PRODUCERMANAGER_H
