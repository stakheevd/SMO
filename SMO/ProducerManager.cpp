#include "ProducerManager.h"

ProducerManager::ProducerManager(LoggerManager* manager, int num_prod, double lamb):
  producers{},
  st_manager(manager),
  number_released_requests(0)
{
	for (int i = 0; i < num_prod; i++)
	{
    producers.push_back(Producer(i+1, lamb));
	}

  select_creating_producer();
}

std::vector<Request*> ProducerManager::get_all_requests() const
{
	std::vector<Request*> vec_requests;

  for (const auto& prod : producers)
    vec_requests.push_back(prod.get_stored_request());

	return vec_requests;
}

Request* ProducerManager::submit_request()
{
  Request* submitted_request = producers[creating_producer_id].send_request();

  select_creating_producer();

	st_manager->request_created(submitted_request);
  number_released_requests++;

	return submitted_request;
}

double ProducerManager::get_creating_producer_time() const
{
  return creating_producer_time;
}

int ProducerManager::get_number_released_requests() const
{
  return number_released_requests;
}

const std::vector<Producer>& ProducerManager::get_producers() const
{
  return producers;
}

void ProducerManager::select_creating_producer()
{
	double min_time = producers[0].get_stored_request()->get_creation_time();
  int prod_id = 0;

  for (size_t i = 1; i < producers.size(); i++)
	{
		if (producers[i].get_stored_request()->get_creation_time() < min_time)
		{
			min_time = producers[i].get_stored_request()->get_creation_time();
      prod_id = i;
		}
	}

  creating_producer_time = min_time;
  creating_producer_id = prod_id;
}
