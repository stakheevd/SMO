#ifndef STATISTICSMANAGER_H
#define STATISTICSMANAGER_H

#include "producerlogger.h"

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

class StatisticsManager
{
public:
  StatisticsManager(int number_producers, int number_consumers);

	void request_created(Request* request);
	void init_received_request(Request* request); // TODO: Rename var
	void log_released_request(Request* request, int cons_id);
	void log_rejected_request(Request* request, double time);

  PivotTable* init_pivot_table(double time);

private:
  std::vector<ProducerLogger> statistics;
  std::vector<double> consumer_working_time;
};

#endif // STATISTICSMANAGER_H
