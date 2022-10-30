#ifndef LOGGERMANAGER_H
#define LOGGERMANAGER_H

#include "Logger.h"

struct PivotTable
{
  std::vector<int> number_requests;
  std::vector<double> probability_rejection;
  std::vector<double> average_elapsed_time;
  std::vector<double> average_waiting_time;
  std::vector<double> average_processing_time;
  std::vector<double> dispersion_waiting_time;
  std::vector<double> dispersion_processing_time;
  std::vector<double> usage_ratio;
};

class LoggerManager
{
public:
  LoggerManager(int number_producers, int number_consumers);

	void request_created(Request* request);
  void log_received_request(Request* request);
	void log_released_request(Request* request, int cons_id);
  void log_rejected_request(Request* request);

  PivotTable* init_pivot_table(double total_simulation_time);

  void setStatistics(const std::vector<Logger>& newStatistics);

private:
  std::vector<Logger> loggers;
  std::vector<double> consumer_working_time;
};

#endif // LOGGERMANAGER_H
