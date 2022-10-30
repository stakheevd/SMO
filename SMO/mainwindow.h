#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTableWidget>
#include <QMainWindow>
#include <QGraphicsScene>
#include <thread>

#include "Simulator.h"
#include "qcustomplot.h"
#include "WaveformGenerator.h"
#include <algorithm>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void fill_producer_data(QVector<double>& vector_number_producers, QVector<double>& vector_prob_reject, QVector<double>& vector_avg_time, QVector<double>& vector_usg_ratio);
  void fill_buffer_data(QVector<double>& vector_number_buffers, QVector<double>& vector_prob_reject, QVector<double>& vector_avg_time, QVector<double>& vector_usg_ratio);
  void fill_consumer_data(QVector<double>& vector_number_consumers, QVector<double>& vector_prob_reject, QVector<double>& vector_avg_time, QVector<double>& vector_usg_ratio);

private slots:
  void start_simulation();
  void on_clear_sim_button_clicked();
  void on_save_conf_button_clicked();
  void on_start_manual_mode_button_clicked();
  void on_start_auto_mode_button_clicked();

private:
  Ui::MainWindow *ui;
  Simulator* simulator;
  WaveformGenerator* wg;
};
#endif // MAINWINDOW_H
