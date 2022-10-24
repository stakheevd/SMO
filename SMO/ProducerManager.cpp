#include "ProducerManager.h"

ProducerManager::ProducerManager(StatisticsManager* manager, int num_prod, double lamb)
{
	producers = std::vector<Producer>();
	st_manager = manager; // TODO: list_init
	released_requests = 0;

	for (int i = 0; i < num_prod; i++)
	{
		producers.push_back(Producer(i+1, lamb));
	}

	// Choose min request
	init_next_event_time();
}

std::vector<Request*> ProducerManager::get_all_requests()
{
	std::vector<Request*> vec_requests;

	for (int i = 0; i < producers.size(); i++)
	{
		vec_requests.push_back(producers[i].get_stored_request());
	}

	return vec_requests;
}

Request* ProducerManager::submit_request()
{
	Request* submitted_request = producers[producer_id].send_request();

	init_next_event_time();

	st_manager->request_created(submitted_request);
	released_requests++;

	return submitted_request;
}

double ProducerManager::get_next_event_time() const
{
  return next_event_time;
}

void ProducerManager::set_next_event_time(double new_next_event_time)
{
  next_event_time = new_next_event_time;
}

int ProducerManager::get_released_requests() const
{
  return released_requests;
}

void ProducerManager::set_released_requests(int new_released_requests)
{
  released_requests = new_released_requests;
}

void ProducerManager::init_next_event_time()
{
	double min_time = producers[0].get_stored_request()->get_creation_time();
	int prod_id = 0;

	for (int i = 1; i < producers.size(); i++)
	{
		if (producers[i].get_stored_request()->get_creation_time() < min_time)
		{
			min_time = producers[i].get_stored_request()->get_creation_time();
			prod_id = i;
		}
	}

	next_event_time = min_time;
	producer_id = prod_id;
}

//void ProducerManager::insert_request(Request req)
//{
  // TODO: Вставка по кольцу
//}

//Request production_manager::emplace_request(Request req)
//{
//  Request deleted_request;
  // Удаление самой старой (меньше всех alive_time) и вставка на её место
//  return deleted_request;
//}
