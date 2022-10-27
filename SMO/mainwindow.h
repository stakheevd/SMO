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

  void fill_producer_data(QVector<double>& v_n_prod, QVector<double>& v_p_r, QVector<double>& v_a_t, QVector<double>& v_u_p);
  void fill_buffer_data(QVector<double>& v_n_bufs, QVector<double>& v_p_r, QVector<double>& v_a_t, QVector<double>& v_u_p);
  void fill_consumer_data(QVector<double>& v_n_cons, QVector<double>& v_p_r, QVector<double>& v_a_t, QVector<double>& v_u_p);

private slots:

  void on_startAutoMode_clicked();

  void on_saveConf_clicked();

  void on_startManualMode_clicked();

  void start_simulation();

  void on_clear_sim_button_clicked();

private:
  Ui::MainWindow *ui;
  // std::unique_ptr
  Simulator* simulator;
  WaveformGenerator* wg;
};
#endif // MAINWINDOW_H
