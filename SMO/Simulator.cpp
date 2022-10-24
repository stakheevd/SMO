#include "Simulator.h"

Simulator::Simulator(int num_requests, int num_prod, int num_buf, int num_cons, double lamb)
{
  std::srand((unsigned)time(NULL)); // TODO: Make init_list?

  consumer_time = 0;
  producer_time = 0;
  number_requests = num_requests;

  manager = new StatisticsManager(num_prod, num_cons);
  consumers = new ConsumerManager(manager, num_cons, lamb);
  buffers = new BufferManager(consumers, manager, num_buf);
  producers = new ProducerManager(manager, num_prod, lamb);
}

StepData *Simulator::get_status()
{
  return new StepData(
        producers->get_all_requests(),
        buffers->get_all_requests(),
        consumers->get_all_requests(),
        std::min(consumer_time, producer_time));
}

void Simulator::take_step()
{
	consumer_time = consumers->get_next_event_time();
	producer_time = producers->get_next_event_time();

	if (consumer_time <= producer_time)
	{
		consumers->release_consumer();

		// Check, there are requests in buffer
		if (buffers->can_submit_request())
		{
			consumers->receive_request(buffers->submit_request());
		}
	}
	else
	{
		buffers->receive_request(producers->submit_request());

		if (consumers->can_receive_request())
		{
			Request* temp_request = buffers->submit_request();
			// TODO: REFACTOR???
			consumers->receive_request(temp_request);
		}
	}
}

PivotTable *Simulator::get_pivot_table()
{
	//return manager->init_pivot_table(std::min(consumer_time, producer_time));
	return manager->init_pivot_table(consumer_time);
}

void Simulator::run_full_simulation()
{
	while (producers->get_released_requests() < number_requests)
	{
		producer_time = producers->get_next_event_time();
		consumer_time = consumers->get_next_event_time();

		if (consumer_time <= producer_time)
		{
			consumers->release_consumer();

			// Check, there are requests in buffer
			if (buffers->can_submit_request()) // Эта проверка работает? И правильно ли увеличивается время?
			{ // HERE!!!
				consumers->receive_request(buffers->submit_request());
			}
		}
		else
		{
			buffers->receive_request(producers->submit_request());

			if (consumers->can_receive_request())
			{
				consumers->receive_request(buffers->submit_request());
			}
		}
	}

	while (!buffers->is_empty() || !consumers->is_empty()) //Refactor: is_empty can_submit
	{

		if (!consumers->is_empty())
		{
			//producer_time = producers->get_next_event_time();
			consumer_time = consumers->get_next_event_time();

			consumers->release_consumer();
		}

		// Check, there are requests in buffer
		if (buffers->can_submit_request()) // Эта проверка работает? И правильно ли увеличивается время?
		{ // HERE!!!
			consumers->receive_request(buffers->submit_request());
		}

	}
}
