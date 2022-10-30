#include "Buffer.h"

Buffer::Buffer(int buf_id):
  buffer_id(buf_id),
  buffered_request(nullptr)
{
}

Request* Buffer::send_request()
{
  Request* submitted_request = buffered_request;
  buffered_request = nullptr;
  return submitted_request;
}

void Buffer::receive_request(Request* req)
{
  buffered_request = req;
}

bool Buffer::is_free() const
{
  return (buffered_request == nullptr);
}

Request *Buffer::get_buffered_request() const
{
  return buffered_request;
}
