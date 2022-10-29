#ifndef CONSUMER_MANAGER_H
#define CONSUMER_MANAGER_H

#include <limits>

#include "consumer.h"
#include "StatisticsManager.h"

class ConsumerManager
{
public:
  ConsumerManager(StatisticsManager* manager, int number_consumers, double lamb);
  std::vector<Request*> get_all_requests();

  void release_consumer();
  bool can_receive_request();
  void receive_request(Request* request);
  const std::vector<Consumer> &get_consumers() const;

  double get_next_event_time() const;
  bool is_empty();

private:
	std::vector<Consumer> consumers;
	StatisticsManager* st_manager;
  int releasing_consumer_id;
	int current_position;
	double next_event_time;
  void select_releasing_consumer();
	void increment_position();
};

#endif // CONSUMER_MANAGER_H
