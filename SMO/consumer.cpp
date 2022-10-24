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
	//std::srand((unsigned)time(NULL));
	double temp_l = -(1.0 / (double)lambda);

	if (temp_l == std::numeric_limits<double>::infinity())
		{
			int p = 5;
			int mm = 0;
		}
	double temp_r = std::log(rand_num_gen()); //std::log(double(std::rand()) / RAND_MAX);

	if (temp_r == std::numeric_limits<double>::infinity())
		{
			int p = 5;
			int mm = 0;
		}

	double delta_t = temp_l * temp_r;
	//double delta_t = (double)(-1 / lambda) * std::log((double)rand() / RAND_MAX);

	if (delta_t == std::numeric_limits<double>::infinity())
		{
			int p = 5;
			int mm = 0;
		}

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

void Consumer::set_current_request(Request *new_current_request)
{
  current_request = new_current_request;
}
  //std::srand((unsigned)time(NULL));

                             //- ((1 / lambda) * std::log(1 - ((double)rand() / RAND_MAX))));
