#include "buffermanager.h"

BufferManager::BufferManager(ConsumerManager* c_manager, StatisticsManager* s_manager, int number_buffers):
  buffers{},
  st_manager(s_manager),
  con_manager(c_manager),
  take_position(0),
  placement_position(0)
{
	for (int i = 0; i < number_buffers; i++)
	{
    buffers.push_back(Buffer(i));
	}
}

std::vector<Request*> BufferManager::get_all_requests()
{
  std::vector<Request*> buffered_requests;

  for (const auto& buf : buffers)
    buffered_requests.push_back(buf.get_buffered_request());

  return buffered_requests;
}

void BufferManager::receive_request(Request *request)
{
  if (!is_full())  //Д1031
  {
    while (!buffers[placement_position].is_free())
      increment_placement_position();

		buffers[placement_position].receive_request(request);
		increment_placement_position();
	}
  else  //Д10О3
  {
    double min_time = std::numeric_limits<double>::max();
    int ind = 0;

    for (size_t i = 0; i < buffers.size(); i++)
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

bool BufferManager::is_full() const
{
  for (const auto& buffer : buffers)
	{
		if (buffer.is_free())
			return false;
	}

	return true;
}

bool BufferManager::is_empty() const
{
  for (const auto& buf : buffers)
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
  while (buffers[take_position].is_free())
    increment_take_position();

  Request* request = buffers[take_position].send_request();
  increment_take_position();

  return request;
}

void BufferManager::increment_placement_position()
{
  placement_position = (placement_position + 1 == (int)buffers.size()) ? 0 : ++placement_position;
}

void BufferManager::increment_take_position()
{
  take_position = (take_position + 1 == (int)buffers.size()) ? 0 : ++take_position;
}
