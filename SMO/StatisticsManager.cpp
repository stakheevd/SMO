#include "StatisticsManager.h"



StatisticsManager::StatisticsManager(int number_producers, int number_consumers)
{
	consumer_working_time = std::vector<double>(number_consumers, 0.0);

	for (int i = 0; i < number_producers; i++)
	{
		statistics.push_back(ProducerLogger());
		}
}

void StatisticsManager::request_created(Request *request)
{
	statistics[request->get_producer_id() - 1].request_created();
}

void StatisticsManager::init_received_request(Request *request)
{
	statistics[request->get_producer_id() - 1].init_received_request(request);
}

void StatisticsManager::log_released_request(Request *request, int cons_id)
{ // ЗДЕСЬ ПАДАЕТ №2
	statistics[request->get_producer_id() - 1].log_released_request(request);

	consumer_working_time[cons_id] += request->get_release_time() - request->get_receiving_time();
}

void StatisticsManager::log_rejected_request(Request *request, double time)
{
	statistics[request->get_producer_id() - 1].log_rejected_request(request, time);
}

PivotTable* StatisticsManager::init_pivot_table(double time)
{
	PivotTable* table = new PivotTable();

	for (auto stat : statistics)
	{
		table->number_requests.push_back(stat.get_total_number_requests());
		table->probability_rejection.push_back(stat.get_probability_rejection());
		table->average_elapsed_time.push_back(stat.get_average_elapsed_time());
		table->average_waiting_time.push_back(stat.get_average_waiting_time());
		table->average_processing_time.push_back(stat.get_average_processing_time());
		table->dispersion_waiting_time.push_back(stat.get_dispersion_waiting_time());
		table->dispersion_processing_time.push_back(stat.get_dispersion_processing_time());
	}

	for (auto cons_time : consumer_working_time)
	{
		table->usage_ratio.push_back(cons_time / time);
	}

	return table;
}



