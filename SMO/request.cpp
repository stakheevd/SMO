#include "request.h"

Request::Request(int r_id, double cr_time, int prod_id):
  id(r_id),
  producer_id(prod_id),
  creation_time(cr_time),
  receiving_time(-1),
  release_time(-1)
{
}

double Request::get_creation_time() const
{
  return creation_time;
}

double Request::get_receiving_time() const
{
  return receiving_time;
}

double Request::get_release_time() const
{
  return release_time;
}

int Request::get_producer_id() const
{
  return producer_id;
}

void Request::set_receiving_time(double time)
{
  receiving_time = time;
}

void Request::set_release_time(double time)
{
  release_time = time;
}

int Request::get_id() const
{
  return id;
}

StepStatus::StepStatus(const std::vector<Request*>& prod_requests, const std::vector<Request*>& buf_requests,
                     const std::vector<Request*>& cons_requests, const double e_time):
  producers_requests(prod_requests),
  buffers_requests(buf_requests),
  consumers_requests(cons_requests),
  elapsed_simulation_time(e_time)
{
}

double StepStatus::get_elapsed_simulation_time() const
{
  return elapsed_simulation_time;
}

const std::vector<Request*>& StepStatus::get_producers_requests() const
{
  return producers_requests;
}

const std::vector<Request*>& StepStatus::get_buffers_requests() const
{
  return buffers_requests;
}

const std::vector<Request*>& StepStatus::get_consumers_requests() const
{
  return consumers_requests;
}
