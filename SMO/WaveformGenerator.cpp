#include "WaveformGenerator.h"

WaveformGenerator::WaveformGenerator(const int number_producers, const int number_buffers, const int number_consumers, StepStatus* init_state, QWidget* parent):
  QGraphicsView(parent),
  producers_lines{},
  buffers_lines{},
  consumers_lines{}
{
  plot = new QGraphicsScene();
  setAlignment(Qt::AlignLeft);

  setScene(plot);

  previous_step_status = new StepStatus(std::vector<Request*> (number_producers, nullptr),
                            std::vector<Request*> (number_buffers, nullptr),
                            std::vector<Request*> (number_consumers, nullptr),
                            0);

  double offset_label = LINE_SPACING_BETWEEN_LABELS;

  create_input_data_lines(number_producers, offset_label, producers_lines, "Источник");
  create_input_data_lines(number_buffers, offset_label, buffers_lines, "Буфер");
  create_input_data_lines(number_consumers, offset_label, consumers_lines, "Прибор");

  reject_requests_line = new QGraphicsItemGroup();
  plot->addItem(reject_requests_line);

  QGraphicsTextItem* reject_label = plot->addText("Отказ");
  reject_requests_line->addToGroup(reject_label);

  reject_label->setPos(10, offset_label);
  reject_label->setScale(1.6);

  draw_step(init_state);
}

void WaveformGenerator::draw_step(StepStatus* step)
{
  double start_x = previous_step_status->get_elapsed_simulation_time() * TIME_TO_PIXELS + MIN_X;
  double end_x = step->get_elapsed_simulation_time() * TIME_TO_PIXELS + MIN_X;

  double current_y = 2 * LINE_SPACING_BETWEEN_LINES - 32;

  draw_type_lines(producers_lines,
                  previous_step_status->get_producers_requests(),
                  step->get_producers_requests(),
                  start_x,
                  current_y,
                  end_x,
                  step->get_elapsed_simulation_time(),
                  true);

  draw_type_lines(buffers_lines,
                  previous_step_status->get_buffers_requests(),
                  step->get_buffers_requests(),
                  start_x,
                  current_y,
                  end_x,
                  step->get_elapsed_simulation_time(),
                  false);

  draw_type_lines(consumers_lines,
                  previous_step_status->get_consumers_requests(),
                  step->get_consumers_requests(),
                  start_x,
                  current_y,
                  end_x,
                  step->get_elapsed_simulation_time(),
                  false);

  reject_requests_line->addToGroup(plot->addLine(start_x, current_y, end_x, current_y, QPen(Qt::gray, 3)));

  for (size_t i = 0; i < buffers_lines.size(); i++)
  {
    // Если в буфере поменялась заявка
    if ((previous_step_status->get_buffers_requests()[i] != nullptr)
          && (previous_step_status->get_buffers_requests()[i] != step->get_buffers_requests()[i]))
    {
      // Если на прибор не попала предыдущая заявка
      if (std::find(step->get_consumers_requests().begin(), step->get_consumers_requests().end(),
           previous_step_status->get_buffers_requests()[i]) == step->get_consumers_requests().end())
      {
        reject_requests_line->addToGroup(plot->addLine(end_x, current_y, end_x, current_y - 10, QPen(Qt::gray, 3)));

        reject_requests_line->addToGroup(add_label(end_x,
                                                   current_y - 35,
                                                   previous_step_status->get_buffers_requests()[i]->get_producer_id(),
                                                   previous_step_status->get_buffers_requests()[i]->get_id()));

        reject_requests_line->addToGroup(add_time_label(end_x, current_y, step->get_elapsed_simulation_time()));
      }
    }
  }

  delete previous_step_status;
  previous_step_status = step;
}

QGraphicsTextItem* WaveformGenerator::add_label(double x, double y, double producer_id, int number)
{
  QGraphicsTextItem* label = plot->addText(QString::fromUtf8(std::to_string(static_cast<int>(producer_id)) + "." + std::to_string(number)));

  label->setPos(x, y);

  return label;
}

QGraphicsTextItem* WaveformGenerator::add_time_label(double x, double y, double timestamp)
{
  QGraphicsTextItem* time_label = plot->addText(QString::fromUtf8(std::to_string(timestamp)));

  time_label->setScale(0.8);
  time_label->setPos(x, y);

  return time_label;
}

void WaveformGenerator::draw_type_lines(std::vector<QGraphicsItemGroup*>& type_lines, const std::vector<Request*>& prev_step_status_requests,
                                          const std::vector<Request*>& cur_step_status_requests, double start_x, double& start_y,
                                            double end_x, double time, bool is_producer)
{
  for (size_t i = 0; i < type_lines.size(); i++)
  {
    // До этого уже был принял сигнал или нет?
    double offset_y = (prev_step_status_requests[i] != nullptr) ? -10 : 0;

    // Сигнал поменял своё состояние?
    if (prev_step_status_requests[i] != cur_step_status_requests[i])
    {
      // Добавляем вертикальную линию
      if (!(is_producer && (time == 0)))
      {
        type_lines[i]->addToGroup(plot->addLine(end_x, start_y, end_x, start_y - 10, QPen(Qt::gray, 3)));
        type_lines[i]->addToGroup(add_time_label(end_x, start_y, time));
      }

      // Если фронт сигнала, добавляем id заявки
      if (cur_step_status_requests[i] != nullptr && time != 0)
      {
        if (is_producer)
        {
          type_lines[i]->addToGroup(add_label(end_x, start_y - 35, cur_step_status_requests[i]->get_producer_id(), prev_step_status_requests[i]->get_id()));
        }
        else
        {
          type_lines[i]->addToGroup(add_label(end_x, start_y - 35, cur_step_status_requests[i]->get_producer_id(), cur_step_status_requests[i]->get_id()));
        }
      }
    }

    if (is_producer)
    {
      type_lines[i]->addToGroup(plot->addLine(start_x, start_y, end_x, start_y, QPen(Qt::gray, 3)));
    }
    else
    {
      type_lines[i]->addToGroup(plot->addLine(start_x, start_y + offset_y, end_x, start_y + offset_y, QPen(Qt::gray, 3)));
    }

    start_y += LINE_SPACING_BETWEEN_LINES;
  }
}

void WaveformGenerator::create_input_data_lines(const int number, double& offset, std::vector<QGraphicsItemGroup*>& lines, const std::string& label)
{
  for (int i = 0; i < number; i++)
  {
    QGraphicsItemGroup* line = new QGraphicsItemGroup();
    lines.push_back(line);

    plot->addItem(line);

    QGraphicsTextItem* label_with_id = plot->addText(QString::fromUtf8(label + " " + std::to_string(i + 1)));

    line->addToGroup(label_with_id);

    label_with_id->setPos(10, offset);
    label_with_id->setScale(1.6);

    offset += LINE_SPACING_BETWEEN_LABELS;
  }
}

QGraphicsScene* WaveformGenerator::get_plot() const
{
  return plot;
}
