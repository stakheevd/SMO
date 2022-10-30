#include "producer.h"

Producer::Producer(int prod_id, double lamb):
  producer_id(prod_id),
  lambda(lamb),
  last_request_id(0),
  stored_request(nullptr)
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
  uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
  rng.seed(ss);
  std::uniform_real_distribution<double> unif(0, 1);

  // Пуассоновский закон распределения
  double delta_t = (-1.0 / (double)lambda) * std::log(unif(rng));
  double previous_creation_time = (stored_request == nullptr) ? 0 : stored_request->get_creation_time();

  stored_request = new Request(++last_request_id, delta_t + previous_creation_time, producer_id);
}

Request *Producer::get_stored_request() const
{
  return stored_request;
}
