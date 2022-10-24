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

  void receive_request(Request* request); // Д1031 и Д1031
  bool can_submit_request(); // empty or full
  [[deprecated]] bool is_full();
  bool is_empty();

  Request* submit_request(); // Д2Б3
private:
  std::vector<Buffer> buffers;
  StatisticsManager* st_manager;
  ConsumerManager* con_manager;
  int take_position;
  int placement_position;
  void select_request(); // Д2Б3

  void increment_take_position();
  void increment_placement_position();

};

#endif // BUFFERMANAGER_H
