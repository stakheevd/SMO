#ifndef REQUEST_H
#define REQUEST_H

#include <vector>

class Request
{
public:
  Request(int r_id, double cr_time, int prod_id);

  //void set_creation_time(double time);
  double get_creation_time() const;
  double get_receiving_time() const;
  double get_release_time() const;
  int get_producer_id() const;

  void set_creation_time(double time);
  void set_receiving_time(double time);
  void set_release_time(double time);

  int getId() const;

private:
  const int id;
  const int producer_id;

  double creation_time;
  double receiving_time;
  double release_time;
};

class StepData
{
public:
  StepData(const std::vector<Request*>& prod_data, const std::vector<Request*>& buf_data, const std::vector<Request*>& cons_data, const double t_stamp);

  double get_timestamp() const;
  void set_timestamp(double new_timestamp);

  const std::vector<Request*>& getProducer_data() const;
  void setProducer_data(const std::vector<Request*>& newProducer_data);

  const std::vector<Request*>& getBuffer_data() const;
  void setBuffer_data(const std::vector<Request*>& newBuffer_data);

  const std::vector<Request*>& getConsumer_data() const;
  void setConsumer_data(const std::vector<Request*>& newConsumer_data);

  std::vector<Request*> producer_data;
  std::vector<Request*> buffer_data;
  std::vector<Request*> consumer_data;
private:
  // TODO: Rename getters and setters
  double timestamp;
};

#endif // REQUEST_H
