#include "LoggerManager.h"

LoggerManager::LoggerManager(int number_producers, int number_consumers):
  consumer_working_time(number_consumers, 0)
{
  for (int i = 0; i < number_producers; i++)
    loggers.push_back(Logger());
}

void LoggerManager::request_created(Request *request)
{
  loggers[request->get_producer_id() - 1].request_created();
}

void LoggerManager::log_received_request(Request *request)
{
  loggers[request->get_producer_id() - 1].log_received_request(request);
}

void LoggerManager::log_released_request(Request *request, int cons_id)
{
  loggers[request->get_producer_id() - 1].log_released_request(request);

	consumer_working_time[cons_id] += request->get_release_time() - request->get_receiving_time();
}

void LoggerManager::log_rejected_request(Request *request)
{
  loggers[request->get_producer_id() - 1].log_rejected_request();
}

PivotTable* LoggerManager::init_pivot_table(double total_simulation_tile)
{
	PivotTable* table = new PivotTable();

  for (const auto& logger : loggers)
	{
    table->number_requests.push_back(logger.get_total_number_requests());
    table->probability_rejection.push_back(logger.get_probability_rejection());
    table->average_elapsed_time.push_back(logger.get_average_elapsed_time());
    table->average_waiting_time.push_back(logger.get_average_waiting_time());
    table->average_processing_time.push_back(logger.get_average_processing_time());
    table->dispersion_waiting_time.push_back(logger.get_dispersion_waiting_time());
    table->dispersion_processing_time.push_back(logger.get_dispersion_processing_time());
	}

  for (const auto& cons_time : consumer_working_time)
	{
    table->usage_ratio.push_back(cons_time / total_simulation_tile);
	}

  return table;
}
