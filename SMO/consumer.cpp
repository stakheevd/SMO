#include "consumer.h"

Consumer::Consumer(int cons_id, double lamb):
  consumer_id(cons_id),
  lambda(lamb),
  previous_time(0),
  current_request(nullptr)
{
}

void Consumer::receive_request(Request* req)
{
  std::mt19937_64 rng;
  uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
  rng.seed(ss);
  std::uniform_real_distribution<double> unif(0, 1);

  double delta_t = -(1.0 / (double)lambda) * std::log(unif(rng));

	if (previous_time > req->get_creation_time())
	{
		// Заявка находилась в буфере (время создания меньше текущего)
		req->set_release_time(previous_time + delta_t);
		req->set_receiving_time(previous_time);
	}
	else
	{
		// Заявка попала из источника сразу в устройство
		req->set_release_time(req->get_creation_time() + delta_t);
		req->set_receiving_time(req->get_creation_time());
	}

	current_request = req;
}

void Consumer::release_consumer()
{
  previous_time = current_request->get_release_time();
  current_request = nullptr;
}

bool Consumer::is_free() const
{
  return (current_request == nullptr);
}

Request *Consumer::get_current_request() const
{
  return current_request;
}
