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
  return producer_id; // ЗДЕСЬ ПАДАЕТ №2
}

void Request::set_creation_time(double time)
{
  creation_time = time;
}

void Request::set_receiving_time(double time)
{
  receiving_time = time;
}

void Request::set_release_time(double time)
{
  release_time = time;
}

StepData::StepData(std::vector<Request*> prod_data, std::vector<Request*> buf_data, std::vector<Request*> cons_data, double t_stamp):
  producer_data(prod_data),
  buffer_data(buf_data),
  consumer_data(cons_data),
  timestamp(t_stamp)
{
}
