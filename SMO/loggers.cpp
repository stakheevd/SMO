#include "loggers.h"

Loggers::Loggers():
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

double Loggers::get_probability_rejection() const
{
	return ((double)rejected_number_requests / total_number_requests);
}

double Loggers::get_average_elapsed_time() const
{
  return ((double)elapsed_time / released_number_requests);
}

double Loggers::get_average_processing_time() const
{
  return ((double)processing_time / released_number_requests);
}

double Loggers::get_average_waiting_time() const
{
  return ((double)waiting_time / released_number_requests);
}

double Loggers::get_dispersion_waiting_time() const
{
  return (((double)squared_waiting_time / released_number_requests)
          - std::pow(get_average_waiting_time(), 2));
}

double Loggers::get_dispersion_processing_time() const
{
  return (((double)squared_processing_time / released_number_requests)
          - std::pow(get_average_processing_time(), 2));
}

void Loggers::request_created()
{
	total_number_requests++;
}

void Loggers::log_received_request(Request* request)
{
	waiting_time += request->get_receiving_time() - request->get_creation_time();
	squared_waiting_time += std::pow(request->get_receiving_time() - request->get_creation_time(), 2);

  elapsed_time += request->get_receiving_time() - request->get_creation_time(); // Сколько было в буфере
}

void Loggers::log_released_request(Request* request)
{
	released_number_requests++;

	processing_time += request->get_release_time() - request->get_receiving_time();
	squared_processing_time += std::pow(request->get_release_time() - request->get_receiving_time(), 2);

  elapsed_time += (request->get_release_time() - request->get_receiving_time()); // Сколько обрабатывалось
  //elapsed_time += (request->get_release_time() - request->get_creation_time());
}

void Loggers::log_rejected_request()
{
	rejected_number_requests++;
}

int Loggers::get_total_number_requests() const
{
  return total_number_requests;
}
