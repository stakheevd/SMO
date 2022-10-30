#ifndef CONSUMER_H
#define CONSUMER_H

#include <random>
#include <cmath>
#include <chrono>

#include "Request.h"

class Consumer
{
public:
  Consumer(int cons_id, double lamb);

  void receive_request(Request* req);
  void release_consumer();

  bool is_free() const;

  Request *get_current_request() const;

private:
  const int consumer_id;
  const double lambda;
  double previous_time;
  Request* current_request;
};

#endif // CONSUMER_H
