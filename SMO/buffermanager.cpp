#include "buffermanager.h"

BufferManager::BufferManager(ConsumerManager* c_manager, StatisticsManager* s_manager, int number_buffers)
{
	buffers = std::vector<Buffer>();

	for (int i = 0; i < number_buffers; i++)
	{
		buffers.push_back(Buffer(i)); // С 0 индекса?
	}

	st_manager = s_manager;
	con_manager = c_manager;
	take_position = 0;
	placement_position = 0;
}

std::vector<Request*> BufferManager::get_all_requests()
{
  std::vector<Request*> buffered_requests;

  for (int i = 0; i < buffers.size(); i++)
  {
    buffered_requests.push_back(buffers[i].get_buffered_request());
  }

  return buffered_requests;
}

void BufferManager::receive_request(Request *request)
{
  if (!is_full()) // change is_full to can_submit...
  {								// Дисциплина буферизации по кольцу (Д1031)
    while (!buffers[placement_position].is_free())
    {
      //increment_position();
      increment_placement_position();
    }

		buffers[placement_position].receive_request(request);
		increment_placement_position();
	}
  else						// Дисциплина отказа самой старой (Д10О3)
  {
    double min_time = std::numeric_limits<double>::max();

    int ind = 0;
    for (int i = 0; i < buffers.size(); i++)
    {
      if (!buffers[i].is_free())
      {
        if (buffers[i].get_buffered_request()->get_creation_time() < min_time)
        {
          min_time = buffers[i].get_buffered_request()->get_creation_time();
          ind = i;
        }
      }
    }

    st_manager->log_rejected_request(request, request->get_creation_time());

    buffers[ind].receive_request(request);
  }
}

bool BufferManager::can_submit_request()
{
	//for (auto buffer : buffers)
	//{
	//	if (!buffer.is_free())
	//		return true;
	//}

	for (int i = 0; i < buffers.size(); i++)
	{
		if (!buffers[i].is_free())
		{
			return true;
		}
	}

	return false;
}

bool BufferManager::is_full()
{
	for (auto buffer : buffers)
	{
		if (buffer.is_free())
			return false;
	}

	return true;
}

bool BufferManager::is_empty()
{
	for (auto buf : buffers)
		{
			if (!buf.is_free())
				{
					return false;
				}
		}

	return true;
}

Request *BufferManager::submit_request()
{
  select_request();
 // ЗДЕСЬ ПАДАЕТ ПРОГРАММА, НЕПРАВИЛЬЫНЙ ВЫБОР БУФЕРА?
  Request request = *buffers[take_position].send_request();
  increment_take_position();

	return new Request(request);
}

void BufferManager::select_request() // TODO: RENAME!!!
{	// ПРАВИЛЬНЫЙ ВАРИАНТ: ВЫБОР ЗАЯВКИ НА ОБСЛУЖИВАНИЕ ПО КОЛЬЦУ Д2Б3
	//for (int i = 0; i < con_manager->get_consumers().size(); i++)
	//	{
	//		if (con_manager->get_consumers()[i].is_free())
	//			{
	//				current_position = i;
	//				break;
	//			}
	//	}
	while (buffers[take_position].is_free())
	{
		//increment_position();
		increment_take_position();
	}


}

void BufferManager::increment_placement_position()
{
	if (placement_position + 1 == buffers.size())
	{
		placement_position = 0;
	}
	else
	{
		placement_position ++;
	}
}

void BufferManager::increment_take_position()
{
	//current_position = ((current_position + 1) == buffers.size()) ? 0 : ++current_position;
	if (take_position + 1 == buffers.size())
	{
		take_position = 0;
	}
	else
	{
		take_position++;
	}

}
