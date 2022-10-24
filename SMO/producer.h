#ifndef PRODUCER_H
#define PRODUCER_H

//#include <random>
#include <cmath>
#include <time.h>
#include <stdlib.h>

#include "RandDouble.h"
#include "request.h"

class Producer
{
public:
  Producer(int prod_id, double lamb);

  Request* send_request();
  void create_request();

  //Request* get_stored_request();
  //void set_stored_request(Request* req);

  Request *get_stored_request() const;
  void setStored_request(Request *new_stored_request);

private:
  const int producer_id;
  const double lambda;
  int last_request_id;
  Request* stored_request;
  Rand_double rand_num_gen;
};

#endif // PRODUCER_H
