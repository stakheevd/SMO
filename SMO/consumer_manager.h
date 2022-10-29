#ifndef CONSUMER_MANAGER_H
#define CONSUMER_MANAGER_H

#include <limits>

#include "consumer.h"
#include "LoggerManager.h"

class ConsumerManager
{
public:
  ConsumerManager(LoggerManager* manager, int number_consumers, double lamb);
  std::vector<Request*> get_all_requests() const;

  void release_consumer();
  void receive_request(Request* request);

  const std::vector<Consumer> &get_consumers() const;
  bool can_receive_request() const;
  double get_releasing_consumer_time() const;
  bool is_empty() const;

private:
	std::vector<Consumer> consumers;
	LoggerManager* st_manager;
  int releasing_consumer_id;
  double releasing_consumer_time;

  void select_releasing_consumer();
};

#endif // CONSUMER_MANAGER_H
