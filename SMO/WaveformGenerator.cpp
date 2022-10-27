#include "WaveformGenerator.h"

WaveformGenerator::WaveformGenerator(const int number_producers, const int number_buffers, const int number_consumers, StepData* init_state, QWidget* parent):
  QGraphicsView(parent)
{
  plot = new QGraphicsScene();
  setAlignment(Qt::AlignLeft);

  setScene(plot);

  previous = new StepData(std::vector<Request*> (number_producers, nullptr),
                          std::vector<Request*> (number_buffers, nullptr),
                          std::vector<Request*> (number_consumers, nullptr), 0);

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

  take_step(init_state);
}

void WaveformGenerator::take_step(StepData* step)
{
  double start_x = previous->get_timestamp() * TIME_TO_PIXELS + MIN_X;
  double end_x = step->get_timestamp() * TIME_TO_PIXELS + MIN_X;

  double current_y = 2 * LINE_SPACING_BETWEEN_LINES - 32;

  make_step_lines(producers_lines, previous->getProducer_data(), step->getProducer_data(), start_x, current_y, end_x, step->get_timestamp());
  make_step_lines(buffers_lines, previous->getBuffer_data(), step->getBuffer_data(), start_x, current_y, end_x, step->get_timestamp());
  make_step_lines(consumers_lines, previous->getConsumer_data(), step->getConsumer_data(), start_x, current_y, end_x, step->get_timestamp());

  reject_requests_line->addToGroup(draw_line(start_x, current_y, end_x, current_y));

  for (size_t i = 0; i < producers_lines.size(); i++)
  {
    if ((previous->getProducer_data()[i] != nullptr) && (previous->getProducer_data()[i] != step->getProducer_data()[i]))
    {
      if (std::find(step->getBuffer_data().begin(), step->getBuffer_data().end(), previous->getProducer_data()[i]) == step->getBuffer_data().end() &&
            std::find(step->getConsumer_data().begin(), step->getConsumer_data().end(), previous->getProducer_data()[i]) == step->getConsumer_data().end())
      {
        reject_requests_line->addToGroup(draw_line(end_x, current_y, end_x, current_y - 10));

        reject_requests_line->addToGroup(add_label(end_x, current_y - 35, previous->getProducer_data()[i]->get_producer_id(), previous->getProducer_data()[i]->getId()));
      }
    }
  }

  for (size_t i = 0; i < buffers_lines.size(); i++)
  {
    if ((previous->getBuffer_data()[i] != nullptr) && (previous->getBuffer_data()[i] != step->getBuffer_data()[i]))
    {
      if (std::find(step->getConsumer_data().begin(), step->getConsumer_data().end(), previous->getBuffer_data()[i]) == step->getConsumer_data().end())
      {
        reject_requests_line->addToGroup(draw_line(end_x, current_y, end_x, current_y - 10));

        reject_requests_line->addToGroup(add_label(end_x, current_y - 35, previous->getBuffer_data()[i]->get_producer_id(), previous->getBuffer_data()[i]->getId()));
      }
    }
  }

  delete previous;
  previous = step;
}

const std::vector<QGraphicsItemGroup*>& WaveformGenerator::get_producers_lines() const
{
  return producers_lines;
}

void WaveformGenerator::set_producers_lines(const std::vector<QGraphicsItemGroup*>& new_producers_lines)
{
  producers_lines = new_producers_lines;
}

inline QGraphicsLineItem* WaveformGenerator::draw_line(double start_x, double start_y, double end_x, double end_y)
{
  return plot->addLine(start_x, start_y, end_x, end_y, QPen(Qt::gray, 3));
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

void WaveformGenerator::make_step_lines(std::vector<QGraphicsItemGroup*>& lines, const std::vector<Request*>& previous, const std::vector<Request*>& current, double start_x, double& start_y, double end_x, double time)
{
  for (size_t i = 0; i < lines.size(); i++)
  {
    double offset_y = ((previous[i] != nullptr) ? -10 : 0); //TODO: Delete brakets

    if (previous[i] != current[i])
    {
      lines[i]->addToGroup(draw_line(end_x, start_y, end_x, start_y - 10));
      // ***** Добавить timestamp'ы на спад сигнала
      //lines[i]->addToGroup(add_time_label(end_x, start_y, time));

      if (current[i] != nullptr)
      {
        lines[i]->addToGroup(add_label(end_x, start_y - 35, current[i]->get_producer_id(), current[i]->getId()));
        lines[i]->addToGroup(add_time_label(end_x, start_y, time));
      }
    }

    lines[i]->addToGroup(draw_line(start_x, start_y + offset_y, end_x, start_y + offset_y));

    start_y += LINE_SPACING_BETWEEN_LINES;
  }
}

void WaveformGenerator::create_input_data_lines(const int number, double& offset, std::vector<QGraphicsItemGroup*>& line_holder, const std::string& label)
{
  for (int i = 0; i < number; i++)
  {
    QGraphicsItemGroup* line = new QGraphicsItemGroup();
    line_holder.push_back(line);

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

void WaveformGenerator::set_plot(QGraphicsScene* new_plot)
{
  plot = new_plot;
}
