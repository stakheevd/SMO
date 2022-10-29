#include "Simulator.h"

Simulator::Simulator(int num_requests, int num_prod,
                       int num_buf, int num_cons, double lamb):
  releasing_consumer_time(0),
  creating_producer_time(0),
  number_requests(num_requests)
{
  loggers = new LoggerManager(num_prod, num_cons);
  consumers = new ConsumerManager(loggers, num_cons, lamb);
  buffers = new BufferManager(loggers, num_buf);
  producers = new ProducerManager(loggers, num_prod, lamb);
}

StepStatus *Simulator::get_step_status()
{
  return new StepStatus(
        producers->get_all_requests(),
        buffers->get_all_requests(),
        consumers->get_all_requests(),
        std::min(releasing_consumer_time, creating_producer_time));
}

void Simulator::take_step()
{
  releasing_consumer_time = consumers->get_releasing_consumer_time();
  creating_producer_time = producers->get_creating_producer_time();

  if (releasing_consumer_time <= creating_producer_time)
	{
		consumers->release_consumer();

    if (!buffers->is_empty())
      consumers->receive_request(buffers->submit_request());
	}
	else
	{
		buffers->receive_request(producers->submit_request());

    if (consumers->can_receive_request())
      consumers->receive_request(buffers->submit_request());
	}
}

PivotTable *Simulator::get_pivot_table()
{
  return loggers->init_pivot_table(releasing_consumer_time);
}

void Simulator::run_full_simulation()
{
  while (producers->get_number_released_requests() < number_requests)
    take_step();

  while (!buffers->is_empty() || !consumers->is_empty())
	{
		if (!consumers->is_empty())
		{
      releasing_consumer_time = consumers->get_releasing_consumer_time();
			consumers->release_consumer();
		}

    if (!buffers->is_empty())
      consumers->receive_request(buffers->submit_request());
  }
}

double Simulator::get_releasing_consumer_time() const
{
  return releasing_consumer_time;
}
