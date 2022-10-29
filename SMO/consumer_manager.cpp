#include "consumer_manager.h"

ConsumerManager::ConsumerManager(StatisticsManager* manager, int number_consumers, double lamb):
  consumers{},
  st_manager(manager),
  current_position(0)
{
	for (int i = 0; i < number_consumers; i++)
	{
		consumers.push_back(Consumer(i + 1, lamb));
	}

  select_releasing_consumer();
}


std::vector<Request*> ConsumerManager::get_all_requests()
{
	std::vector<Request*> received_requests;

  for (auto req : consumers)
  {
    received_requests.push_back(req.get_current_request());
  }

	return received_requests;
}

void ConsumerManager::release_consumer()
{
  st_manager->log_released_request(consumers[releasing_consumer_id].get_current_request(), releasing_consumer_id);

  consumers[releasing_consumer_id].release_consumer();

  select_releasing_consumer();
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
{ // Д2П1
  for (size_t i = 0; i < consumers.size(); i++)
  {
    if (consumers[i].is_free())
    {
      consumers[i].receive_request(request);

      st_manager->log_received_request(request);

      select_releasing_consumer();
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
  for (const auto& con : consumers)
		{
			if (!con.is_free())
				{
					return false;
				}
		}
	return true;
}

void ConsumerManager::select_releasing_consumer()
{
	double min_time = std::numeric_limits<double>::max();

  for (size_t i = 0; i < consumers.size(); i++)
		{
			if (!consumers[i].is_free())
				{
					if (min_time > consumers[i].get_current_request()->get_release_time())
						{
							min_time = consumers[i].get_current_request()->get_release_time();
              releasing_consumer_id = i;
						}
				}
		}

	next_event_time = min_time;
}
