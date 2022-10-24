#ifndef CONSUMER_H
#define CONSUMER_H

#include <random>
#include <time.h>
#include <cmath>
#include <stdlib.h>


#include "RandDouble.h"
#include "request.h"

class Consumer
{
public:
  Consumer(int cons_id, double lamb);

  void receive_request(Request* req);
  void release_consumer();

  bool is_free() const;

  Request *get_current_request() const;
  void set_current_request(Request *new_current_request);

private:
  const int consumer_id;
  const double lambda;
  double previous_time;
  Request* current_request;
  Rand_double rand_num_gen;
};

#endif // CONSUMER_H
