#ifndef PRODUCER_H
#define PRODUCER_H

#include <cmath>
#include <chrono>
#include <random>

#include "Request.h"

class Producer
{
public:
  Producer(int prod_id, double lamb);

  Request* send_request();

  Request *get_stored_request() const;

private:
  const int producer_id;
  const double lambda;
  int last_request_id;
  Request* stored_request;

  void create_request();
};

#endif // PRODUCER_H
