#include "producerlogger.h"

ProducerLogger::ProducerLogger():
  total_number_requests(0),
  rejected_number_requests(0),
  released_number_requests(0),
  elapsed_time(0),
  waiting_time(0),
  squared_waiting_time(0),
  processing_time(0),
  squared_processing_time(0)
{
}

double ProducerLogger::get_probability_rejection() const
{
	return ((double)rejected_number_requests / total_number_requests);
}

double ProducerLogger::get_average_elapsed_time() const
{
	return ((double)elapsed_time / total_number_requests);
}

double ProducerLogger::get_average_processing_time() const
{
	return ((double)processing_time / released_number_requests); // total_number?
}

double ProducerLogger::get_average_waiting_time() const
{
	return ((double)waiting_time / released_number_requests); // total_num?
}

double ProducerLogger::get_dispersion_waiting_time() const
{
	return (((double)squared_waiting_time / released_number_requests)
					- std::pow(get_average_waiting_time(), 2));
}

double ProducerLogger::get_dispersion_processing_time() const
{
	return (((double)squared_processing_time / released_number_requests)
					- std::pow(get_average_processing_time(), 2));
}

double ProducerLogger::get_usage_ratio(double time) const
{
	return (processing_time / time);
}

void ProducerLogger::request_created()
{
	total_number_requests++;
}

void ProducerLogger::init_received_request(Request* request)
{
	waiting_time += request->get_receiving_time() - request->get_creation_time();
	squared_waiting_time += std::pow(request->get_receiving_time() - request->get_creation_time(), 2);
}

void ProducerLogger::log_released_request(Request* request) // WTF??? 2 6 14
{
	released_number_requests++;


	processing_time += request->get_release_time() - request->get_receiving_time();
	squared_processing_time += std::pow(request->get_release_time() - request->get_receiving_time(), 2);

	elapsed_time += request->get_release_time() - request->get_creation_time();
}

void ProducerLogger::log_rejected_request(Request* request, double elaps_time) // WTF??
{
	rejected_number_requests++;

	elapsed_time += elaps_time - request->get_creation_time();
}

int ProducerLogger::get_total_number_requests() const
{
  return total_number_requests;
}

void ProducerLogger::set_total_number_requests(int new_total_number_requests)
{
  total_number_requests = new_total_number_requests;
}

int ProducerLogger::get_rejected_number_requests() const
{
  return rejected_number_requests;
}

void ProducerLogger::set_rejected_number_requests(int new_rejected_number_requests)
{
  rejected_number_requests = new_rejected_number_requests;
}

int ProducerLogger::get_released_number_requests() const
{
  return released_number_requests;
}

void ProducerLogger::set_released_number_requests(int new_released_number_requests)
{
  released_number_requests = new_released_number_requests;
}

double ProducerLogger::get_squared_waiting_time() const
{
  return squared_waiting_time;
}

void ProducerLogger::set_squared_waiting_time(double new_squared_waiting_time)
{
  squared_waiting_time = new_squared_waiting_time;
}

double ProducerLogger::get_squared_processing_time() const
{
  return squared_processing_time;
}

void ProducerLogger::set_squared_processing_time(double new_squared_processing_time)
{
  squared_processing_time = new_squared_processing_time;
}
