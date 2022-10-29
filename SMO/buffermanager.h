#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include <limits>

#include "buffer.h"
#include "LoggerManager.h"

class BufferManager
{
public:
  BufferManager(LoggerManager* s_manager, int number_buffers);

  std::vector<Request*> get_all_requests() const;
  Request* submit_request();
  void receive_request(Request* request);

  bool is_full() const;
  bool is_empty() const;

private:
  std::vector<Buffer> buffers;
  LoggerManager* st_manager;
  int take_position;
  int placement_position;

  void increment_take_position();
  void increment_placement_position();
};

#endif // BUFFERMANAGER_H
