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

int Request::getId() const
{
  return id;
}
// TODO: Refactor size of line
StepData::StepData(const std::vector<Request*>& prod_data, const std::vector<Request*>& buf_data, const std::vector<Request*>& cons_data, const double t_stamp):
  producer_data(prod_data),
  buffer_data(buf_data),
  consumer_data(cons_data),
  timestamp(t_stamp)
{
}

double StepData::get_timestamp() const
{
  return timestamp;
}

void StepData::set_timestamp(double new_timestamp)
{
  timestamp = new_timestamp;
}

const std::vector<Request*>& StepData::getProducer_data() const
{
  return producer_data;
}

void StepData::setProducer_data(const std::vector<Request*>& newProducer_data)
{
  producer_data = newProducer_data;
}

const std::vector<Request*>& StepData::getBuffer_data() const
{
  return buffer_data;
}

void StepData::setBuffer_data(const std::vector<Request*>& newBuffer_data)
{
  buffer_data = newBuffer_data;
}

const std::vector<Request*>& StepData::getConsumer_data() const
{
  return consumer_data;
}

void StepData::setConsumer_data(const std::vector<Request*>& newConsumer_data)
{
  consumer_data = newConsumer_data;
}
