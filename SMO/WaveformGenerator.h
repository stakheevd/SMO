#ifndef WAVEFORMGENERATOR_H
#define WAVEFORMGENERATOR_H

#include <QGraphicsView>
#include <QGraphicsItemGroup>
#include <string>
#include <algorithm>

#include "request.h"

class WaveformGenerator : public QGraphicsView
{
public:
  WaveformGenerator(const int number_producers, const int number_buffers, const int number_consumers, StepData* init_state, QWidget* parent = 0);
  // TODO: Зачем наследуемся от QGraphicsView?

  void take_step(StepData* step);

  QGraphicsScene* get_plot() const;
  void set_plot(QGraphicsScene* new_plot);

  const std::vector<QGraphicsItemGroup*>& get_producers_lines() const;
  void set_producers_lines(const std::vector<QGraphicsItemGroup*>& new_producers_lines);

private:
  StepData* previous = nullptr;

  QGraphicsScene* plot;

  std::vector<QGraphicsItemGroup*> producers_lines;
  std::vector<QGraphicsItemGroup*> buffers_lines;
  std::vector<QGraphicsItemGroup*> consumers_lines;
  QGraphicsItemGroup* reject_requests_line;
  // Может прошедшее время в виде линии с timestamps?

  // TODO: RENAME consts
  const int LINE_SPACING_BETWEEN_LABELS = 60;
  const int MIN_X = 150;
  const double LINE_SPACING_BETWEEN_LINES = 60;
  const double TIME_TO_PIXELS = 1000;

  inline QGraphicsLineItem* draw_line(double start_x, double start_y, double end_x, double end_y);
  QGraphicsTextItem* add_label(double x, double y, double producer_id, int number);
  QGraphicsTextItem* add_time_label(double x, double y, double timestamp);

  void make_step_lines(std::vector<QGraphicsItemGroup*>& lines, const std::vector<Request*>& previous,
                         const std::vector<Request*>& current, double start_x, double& start_y, double end_x, double time, bool is_producer);
  // TODO: Line_holder?? What is it?
  void create_input_data_lines(const int number, double& offset, std::vector<QGraphicsItemGroup*>& line_holder, const std::string& label);
};

#endif // WAVEFORMGENERATOR_H
