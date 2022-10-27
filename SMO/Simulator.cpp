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
    if (!buffers->is_empty())
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
      if (!buffers->is_empty()) // Эта проверка работает? И правильно ли увеличивается время?
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
    if (!buffers->is_empty()) // Эта проверка работает? И правильно ли увеличивается время?
		{ // HERE!!!
			consumers->receive_request(buffers->submit_request());
		}

  }
}

// TODO: Rename
ProducerManager* Simulator::getProducers() const
{
  return producers;
}

void Simulator::setProducers(ProducerManager* newProducers)
{
  producers = newProducers;
}
// TODO: Rename
double Simulator::getConsumer_time() const
{
  return consumer_time;
}

void Simulator::setConsumer_time(double newConsumer_time)
{
  consumer_time = newConsumer_time;
}

BufferManager* Simulator::getBuffers() const
{
  return buffers;
}

void Simulator::setBuffers(BufferManager* newBuffers)
{
  buffers = newBuffers;
}

ConsumerManager* Simulator::getConsumers() const
{
  return consumers;
}

void Simulator::setConsumers(ConsumerManager* newConsumers)
{
  consumers = newConsumers;
}

StatisticsManager* Simulator::getManager() const
{
  return manager;
}

void Simulator::setManager(StatisticsManager* newManager)
{
  manager = newManager;
}

int Simulator::getNumber_requests() const
{
  return number_requests;
}

void Simulator::setNumber_requests(int newNumber_requests)
{
  number_requests = newNumber_requests;
}

double Simulator::getProducer_time() const
{
  return producer_time;
}

void Simulator::setProducer_time(double newProducer_time)
{
  producer_time = newProducer_time;
}
