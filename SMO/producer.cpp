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
  //std::srand((unsigned)time(NULL));
  double delta_t = (-1.0 / (double)lambda) * std::log(rand_num_gen());;
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
