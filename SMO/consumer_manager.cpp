#include "consumer_manager.h"

ConsumerManager::ConsumerManager(StatisticsManager* manager, int number_consumers, double lamb)
{
	consumers = std::vector<Consumer>();
	current_position = 0;

	for (int i = 0; i < number_consumers; i++)
	{
		consumers.push_back(Consumer(i + 1, lamb));
	}

	st_manager = manager;

  select_consumer_by_priority();
}


std::vector<Request*> ConsumerManager::get_all_requests()
{
	std::vector<Request*> received_requests;
	// TODO: Сделать for each через auto
  //for (int i = 0; i < consumers.size(); i++)
  //{
  //	received_requests.push_back(consumers[i].get_current_request());
  //}

  for (auto req : consumers)
  {
    received_requests.push_back(req.get_current_request());
  }

	return received_requests;
}

void ConsumerManager::release_consumer()
{
	st_manager->log_released_request(consumers[next_free_consumer_id].get_current_request(), next_free_consumer_id);

	consumers[next_free_consumer_id].release_consumer();

  select_consumer_by_priority();
}

bool ConsumerManager::can_receive_request()
{
	for (const auto& consumer : consumers)
	{
		if (consumer.is_free())
			return true;
	}

	return false;
}

void ConsumerManager::receive_request(Request* request)
{ // Д2П1 TODO: Сделать for each через auto
	for (int i = 0; i < consumers.size(); i++)
	{
		if (consumers[i].is_free())
		{
			consumers[i].receive_request(request);

			st_manager->init_received_request(request);

      select_consumer_by_priority();
			return;
		}
	}
}

const std::vector<Consumer> &ConsumerManager::get_consumers() const
{
	return consumers;
}

double ConsumerManager::get_next_event_time() const
{
	return next_event_time;
}

bool ConsumerManager::is_empty()
{
	for (const auto& con : consumers) // Нужен ли здесь &?
		{
			if (!con.is_free())
				{
					return false;
				}
		}
	return true;
}

void ConsumerManager::select_consumer_by_priority()
{
	double min_time = std::numeric_limits<double>::max();

	for (int i = 0; i < consumers.size(); i++)
		{
			if (!consumers[i].is_free())
				{
					if (min_time > consumers[i].get_current_request()->get_release_time())
						{
							min_time = consumers[i].get_current_request()->get_release_time();
							next_free_consumer_id = i;
						}
				}
		}

	next_event_time = min_time;
}

void ConsumerManager::increment_position()
{
	current_position = ((current_position + 1) == consumers.size()) ? 0 : ++current_position;
}
