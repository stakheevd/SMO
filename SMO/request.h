#ifndef REQUEST_H
#define REQUEST_H

#include <vector>

class Request
{
public:
  Request(int r_id, double cr_time, int prod_id);

  double get_creation_time() const;
  double get_receiving_time() const;
  double get_release_time() const;
  int get_producer_id() const;
  int get_id() const;

  void set_receiving_time(double time);
  void set_release_time(double time);

private:
  const int id;
  const int producer_id;

  const double creation_time;
  double receiving_time;
  double release_time;
};

class StepData
{
public:
  StepData(const std::vector<Request*>& prod_requests, const std::vector<Request*>& buf_requests,
             const std::vector<Request*>& cons_requests, const double e_time);

  double get_elapsed_simulation_time() const;
  const std::vector<Request*>& get_producers_requests() const;
  const std::vector<Request*>& get_buffers_requests() const;
  const std::vector<Request*>& get_consumers_requests() const;

private:
  std::vector<Request*> producers_requests;
  std::vector<Request*> buffers_requests;
  std::vector<Request*> consumers_requests;
  double elapsed_simulation_time;
};

#endif // REQUEST_H
