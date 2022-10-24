#ifndef WAVEFORMGENERATOR_H
#define WAVEFORMGENERATOR_H

#include <QGraphicsView>
#include <QGraphicsItemGroup>

#include "request.h"

class WaveformGenerator
{
public:
  WaveformGenerator();
  // TODO: Перенести это всё в private
  QGraphicsScene* plot;

  std::vector<QGraphicsItemGroup*> producers_lines;
  std::vector<QGraphicsItemGroup*> buffers_lines;
  std::vector<QGraphicsItemGroup*> consumers_lines;
  QGraphicsItemGroup* reject_requests_line;

  void take_step(StepData* step);

private:
  StepData* previous = nullptr;

  // TODO: RENAME consts
  const int SPACE = 38;
  const int MIX_X = 150;
  const double SPACE_LINES = 38;
  const double TIME_TO_PIXELS = 1000;

  inline QGraphicsLineItem* draw_line(double start_x, double start_y, double end_x, double end_y);
  QGraphicsTextItem* add_label(double x, double y, double producer_id, int number);

	void make_step_lines(std::vector<QGraphicsItemGroup*>& lines, std::vector<Request*>& previous,
											 std::vector<Request*>& current, double start_x, double& start_y, double end_x);
	void create_input_data_lines();
};

#endif // WAVEFORMGENERATOR_H
