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
  StepData(std::vector<Request*> prod_data, std::vector<Request*> buf_data, std::vector<Request*> cons_data, double t_stamp);

private:
  std::vector<Request*> producer_data;
  std::vector<Request*> buffer_data;
  std::vector<Request*> consumer_data;
  double timestamp;
};

#endif // REQUEST_H
