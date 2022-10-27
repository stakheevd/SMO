#ifndef BUFFER_H
#define BUFFER_H

#include "request.h"

class Buffer
{
public:
  Buffer(int buf_id);

  Request* send_request();
  void receive_request(Request* req);

  bool is_free() const;

  Request *get_buffered_request() const;

private:
  const int buffer_id;
  Request* buffered_request;
};

#endif // BUFFER_H
