#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include <limits>

#include "buffer.h"
#include "consumer_manager.h"
#include "StatisticsManager.h"

class BufferManager
{
public:
  BufferManager(ConsumerManager* c_manager, StatisticsManager* s_manager, int number_buffers);

  std::vector<Request*> get_all_requests();
  Request* submit_request();
  void receive_request(Request* request);

  bool is_full() const;
  bool is_empty() const;

private:
  std::vector<Buffer> buffers;
  StatisticsManager* st_manager;
  ConsumerManager* con_manager;
  int take_position;
  int placement_position;

  void increment_take_position();
  void increment_placement_position();
};

#endif // BUFFERMANAGER_H
