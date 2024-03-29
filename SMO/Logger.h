#ifndef LOGGER_H
#define LOGGER_H

#include <cmath>

#include "Request.h"

class Logger
{
public:
  Logger();

  int get_total_number_requests() const;
  double get_probability_rejection() const;
	double get_average_elapsed_time() const;
	double get_average_processing_time() const;
	double get_average_waiting_time() const;
	double get_dispersion_waiting_time() const;
	double get_dispersion_processing_time() const;

	void request_created();
  void log_received_request(Request* request);
	void log_released_request(Request* request);
  void log_rejected_request();

private:
  int total_number_requests;
  int rejected_number_requests;
  int released_number_requests;

  double elapsed_time;
  double waiting_time;
  double squared_waiting_time;
  double processing_time;
  double squared_processing_time;
};

#endif // LOGGER_H
