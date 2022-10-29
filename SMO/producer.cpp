#include "producer.h"

Producer::Producer(int prod_id, double lamb):
  stored_request(nullptr),
  producer_id(prod_id),
  lambda(lamb),
  last_request_id(0)
{
  create_request();
}

Request* Producer::send_request()
{
  Request* submitted_request = stored_request;
  create_request();

  return submitted_request;
}

void Producer::create_request()
{
  std::mt19937_64 rng;
  // initialize the random number generator with time-dependent seed
  uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
  rng.seed(ss);
  // initialize a uniform distribution between 0 and 1
  std::uniform_real_distribution<double> unif(0, 1);

  double delta_t = (-1.0 / (double)lambda) * std::log(unif(rng));
  //double delta_t = (-1.0 / (double)lambda) * std::log(rand_num_gen());
  //double delta_t = (-1 / lambda) * std::log((double)rand() / RAND_MAX);
  double previous_time = (stored_request == nullptr) ? 0 : stored_request->get_creation_time();
  stored_request = new Request(++last_request_id, delta_t + previous_time, producer_id);
}

Request *Producer::get_stored_request() const
{
  return stored_request;
}

//void Producer::set_stored_request(Request *new_stored_request)
//{
//  stored_request = new_stored_request;
//}

//Request* Producer::get_stored_request()
//{
//  return stored_request;
//}
