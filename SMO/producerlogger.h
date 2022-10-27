#ifndef PRODUCERLOGGER_H
#define PRODUCERLOGGER_H

#include <cmath>

#include "request.h"

class ProducerLogger
{
public:
	ProducerLogger();

	double get_probability_rejection() const;
	double get_average_elapsed_time() const;
	double get_average_processing_time() const;
	double get_average_waiting_time() const;
	double get_dispersion_waiting_time() const;
	double get_dispersion_processing_time() const;
	double get_usage_ratio(double time) const;

	void request_created();
	void init_received_request(Request* request); // TODO: Rename var
	void log_released_request(Request* request);
	void log_rejected_request(Request* request, double elaps_time);

  int get_total_number_requests() const;
  void set_total_number_requests(int new_total_number_requests);

  int get_rejected_number_requests() const;
  void set_rejected_number_requests(int new_rejected_number_requests);

  int get_released_number_requests() const;
  void set_released_number_requests(int new_released_number_requests);

  double get_squared_waiting_time() const;
  void set_squared_waiting_time(double new_squared_waiting_time);

  double get_squared_processing_time() const;
  void set_squared_processing_time(double new_squared_processing_time);

private:
  int total_number_requests;
  int rejected_number_requests;
  int released_number_requests;

  double elapsed_time;
  double waiting_time;
  double squared_waiting_time;
  double processing_time; // Это среднее время обработки на приборах, осёл
  double squared_processing_time;
};

#endif // PRODUCERLOGGER_H