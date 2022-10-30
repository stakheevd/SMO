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
  WaveformGenerator(const int number_producers, const int number_buffers, const int number_consumers, StepStatus* init_state, QWidget* parent = 0);

  void draw_step(StepStatus* step);

  QGraphicsScene* get_plot() const;

private:
  StepStatus* previous_step_status;
  QGraphicsScene* plot;

  std::vector<QGraphicsItemGroup*> producers_lines;
  std::vector<QGraphicsItemGroup*> buffers_lines;
  std::vector<QGraphicsItemGroup*> consumers_lines;
  QGraphicsItemGroup* reject_requests_line;

  const int LINE_SPACING_BETWEEN_LABELS = 60;
  const int MIN_X = 150;
  const double LINE_SPACING_BETWEEN_LINES = 60;
  const double TIME_TO_PIXELS = 1000;

  QGraphicsTextItem* add_label(double x, double y, double producer_id, int number);
  QGraphicsTextItem* add_time_label(double x, double y, double timestamp);

  void draw_type_lines(std::vector<QGraphicsItemGroup*>& type_lines, const std::vector<Request*>& prev_step_status_requests,
                         const std::vector<Request*>& cur_step_status_requests, double start_x, double& start_y,
                           double end_x, double time, bool is_producer);

  void create_input_data_lines(const int number, double& offset, std::vector<QGraphicsItemGroup*>& line_holder, const std::string& label);
};

#endif // WAVEFORMGENERATOR_H
