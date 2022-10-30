#include "mainwindow.h"

#include <QString>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_save_conf_button_clicked()
{
   ui->statusbar->showMessage("Конфигурация сохранена");
   ui->tabWidget->widget(0)->setDisabled(true);
   ui->tabWidget->widget(1)->setEnabled(true);
   ui->tabWidget->widget(2)->setEnabled(true);

  simulator = new Simulator(ui->number_requests->value(), ui->number_producers->value(), ui->buffer_size->value(), ui->number_consumers->value(), ui->lamb->value());
  wg = new WaveformGenerator(ui->number_producers->value(), ui->buffer_size->value(), ui->number_consumers->value(), simulator->get_step_status());

  ui->graphicsView->setScene(wg->get_plot());
}

void MainWindow::on_start_auto_mode_button_clicked()
{
   ui->statusbar->setVisible(false);
   ui->start_auto_mode_button->setVisible(false);
   start_simulation();
}

void MainWindow::on_start_manual_mode_button_clicked()
{
  ui->statusbar->setVisible(false);
  simulator->take_step();
  wg->draw_step(simulator->get_step_status());
}

void MainWindow::start_simulation()
{
  if (simulator->get_releasing_consumer_time() > 0)
  {
    delete simulator;
    simulator = new Simulator(ui->number_requests->value(), ui->number_producers->value(), ui->buffer_size->value(), ui->number_consumers->value(), ui->lamb->value());
  }

  ui->start_manual_mode_button->setEnabled(false);
  ui->start_manual_mode_button->setText("Симуляция завершена");

  // Для разного кол-ва источников
  QVector<double> vec_num_producers(80);
  QVector<double> vec_prob_rej_by_prod;
  QVector<double> vec_avg_time_by_prod;
  QVector<double> vec_usage_produces_by_prod;

  vec_prob_rej_by_prod.reserve(80);
  vec_avg_time_by_prod.reserve(80);
  vec_usage_produces_by_prod.reserve(80);

  std::thread prod_data([&]()
  {
    fill_producer_data(vec_num_producers, vec_prob_rej_by_prod, vec_avg_time_by_prod, vec_usage_produces_by_prod);
  });

  // Для разного кол-ва буферов
  QVector<double> vec_num_buffers(80);
  QVector<double> vec_prob_rej_by_buffers;
  QVector<double> vec_avg_time_by_buffers;
  QVector<double> vec_usage_produces_by_buffers;

  vec_prob_rej_by_buffers.reserve(80);
  vec_avg_time_by_buffers.reserve(80);
  vec_usage_produces_by_buffers.reserve(80);

  std::thread buf_data([&]()
  {
    fill_buffer_data(vec_num_buffers, vec_prob_rej_by_buffers, vec_avg_time_by_buffers, vec_usage_produces_by_buffers);
  });

  // Для разного кол-ва приборов
  QVector<double> vec_num_consumers(80);
  QVector<double> vec_prob_rej_by_consumers;
  QVector<double> vec_avg_time_by_consumers;
  QVector<double> vec_usage_produces_by_consumers;

  vec_prob_rej_by_consumers.reserve(80);
  vec_avg_time_by_consumers.reserve(80);
  vec_usage_produces_by_consumers.reserve(80);

  std::thread cons_data([&]()
  {
    fill_consumer_data(vec_num_consumers, vec_prob_rej_by_consumers, vec_avg_time_by_consumers, vec_usage_produces_by_consumers);
  });

  prod_data.join();
  buf_data.join();
  cons_data.join();

  QCustomPlot* plot= ui->prob_by_prod;
  QCPCurve* line= new QCPCurve(plot ->xAxis, plot ->yAxis);
  plot->xAxis->setLabel("Количество источников");
  plot->yAxis->setLabel("Вероятность отказа");
  line->setData(vec_num_producers, vec_prob_rej_by_prod);
  plot->rescaleAxes();
  plot->replot();

  plot = ui->time_by_prod;
  line = new QCPCurve(plot ->xAxis, plot ->yAxis);
  plot->xAxis->setLabel("Количество источников");
  plot->yAxis->setLabel("Среднее время в с-ме");
  line->setData(vec_num_producers, vec_avg_time_by_prod);
  plot->rescaleAxes();
  plot->replot();

  plot = ui->us_by_prod;
  line = new QCPCurve(plot ->xAxis, plot ->yAxis);
  plot->xAxis->setLabel("Количество источников");
  plot->yAxis->setLabel("Загруженность приборов");
  line->setData(vec_num_producers, vec_usage_produces_by_prod);
  plot->rescaleAxes();
  plot->replot();

  plot = ui->prob_by_buf;
  line = new QCPCurve(plot ->xAxis, plot ->yAxis);
  plot->xAxis->setLabel("Количество буферов");
  plot->yAxis->setLabel("Вероятность отказа");
  line->setData(vec_num_buffers, vec_prob_rej_by_buffers);
  plot->rescaleAxes();
  plot->replot();

  plot = ui->time_by_buf;
  line = new QCPCurve(plot ->xAxis, plot ->yAxis);
  plot->xAxis->setLabel("Количество буферов");
  plot->yAxis->setLabel("Среднее время в с-ме");
  line->setData(vec_num_buffers, vec_avg_time_by_buffers);
  plot->rescaleAxes();
  plot->replot();

  plot = ui->us_by_buf;
  line = new QCPCurve(plot ->xAxis, plot ->yAxis);
  plot->xAxis->setLabel("Количество буферов");
  plot->yAxis->setLabel("Загруженность приборов");
  line->setData(vec_num_buffers, vec_usage_produces_by_buffers);
  plot->rescaleAxes();
  plot->replot();

  plot = ui->prob_by_cons;
  line = new QCPCurve(plot ->xAxis, plot ->yAxis);
  plot->xAxis->setLabel("Количество приборов");
  plot->yAxis->setLabel("Вероятность отказа");
  line->setData(vec_num_consumers, vec_prob_rej_by_consumers);
  plot->rescaleAxes();
  plot->replot();

  plot = ui->time_by_cons;
  line = new QCPCurve(plot ->xAxis, plot ->yAxis);
  plot->xAxis->setLabel("Количество приборов");
  plot->yAxis->setLabel("Среднее время в с-ме");
  line->setData(vec_num_consumers, vec_avg_time_by_consumers);
  plot->rescaleAxes();
  plot->replot();

  plot = ui->us_by_cons;
  line = new QCPCurve(plot ->xAxis, plot ->yAxis);
  plot->xAxis->setLabel("Количество приборов");
  plot->yAxis->setLabel("Загруженность приборов");
  line->setData(vec_num_consumers, vec_usage_produces_by_consumers);
  plot->rescaleAxes();
  plot->replot();

  // Симуляция для входных данных
  simulator->run_full_simulation();
  PivotTable* table = simulator->get_pivot_table();

  QTableWidget* main_table = ui->tableWidget;
  QTableWidget* us_table = ui->tableWidget_2;

  main_table->setRowCount(ui->number_producers->value());
  us_table->setRowCount(ui->number_consumers->value());

  for (int i = 0; i < ui->number_producers->value(); i++)
  {
    main_table->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
    main_table->setItem(i, 1, new QTableWidgetItem(QString::number(table->number_requests[i])));
    main_table->setItem(i, 2, new QTableWidgetItem(QString::number(table->probability_rejection[i])));
    main_table->setItem(i, 3, new QTableWidgetItem(QString::number(table->average_elapsed_time[i])));
    main_table->setItem(i, 4, new QTableWidgetItem(QString::number(table->average_waiting_time[i])));
    main_table->setItem(i, 5, new QTableWidgetItem(QString::number(table->average_processing_time[i])));
    main_table->setItem(i, 6, new QTableWidgetItem(QString::number(table->dispersion_waiting_time[i])));
    main_table->setItem(i, 7, new QTableWidgetItem(QString::number(table->dispersion_processing_time[i])));
  }

  for (int i = 0; i < ui->number_consumers->value(); i++)
  {
    us_table->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
    us_table->setItem(i, 1, new QTableWidgetItem(QString::number(table->usage_ratio[i])));
  }
}

void MainWindow::fill_producer_data(QVector<double>& vector_number_producers, QVector<double>& vector_prob_reject, QVector<double>& vector_avg_time, QVector<double>& vector_usg_ratio)
{
	Simulator* temp_simulator_1;
	PivotTable* temp_table_1;

  for (int i = 1; i <= vector_number_producers.size(); i++)
  {
		temp_simulator_1 = new Simulator(4000, i, 4, 4, 3.0);
		temp_simulator_1->run_full_simulation();
		temp_table_1 = temp_simulator_1->get_pivot_table();

    vector_number_producers[i - 1] = i;
    vector_prob_reject.push_back(*std::max_element(temp_table_1 ->probability_rejection.begin(), temp_table_1->probability_rejection.end()));
    vector_avg_time.push_back(*std::max_element(temp_table_1->average_elapsed_time.begin(), temp_table_1->average_elapsed_time.end()));
    vector_usg_ratio.push_back(*std::min_element(temp_table_1->usage_ratio.begin(), temp_table_1->usage_ratio.end()));

		delete temp_simulator_1;
		delete temp_table_1;
	}
}

void MainWindow::fill_buffer_data(QVector<double>& vector_number_buffers, QVector<double>& vector_prob_reject, QVector<double>& vector_avg_time, QVector<double>& vector_usg_ratio)
{
		Simulator* temp_simulator_2;
		PivotTable* temp_table_2;

    for (int i = 1; i <= vector_number_buffers.size(); i++)
    {
			temp_simulator_2 = new Simulator(4000, 4, i, 4, 3.0);
			temp_simulator_2->run_full_simulation();
			temp_table_2 = temp_simulator_2->get_pivot_table();

      vector_number_buffers[i - 1] = i;
      vector_prob_reject.push_back(*std::max_element(temp_table_2->probability_rejection.begin(), temp_table_2->probability_rejection.end()));
      vector_avg_time.push_back(*std::max_element(temp_table_2->average_elapsed_time.begin(), temp_table_2->average_elapsed_time.end()));
      vector_usg_ratio.push_back(*std::min_element(temp_table_2->usage_ratio.begin(), temp_table_2->usage_ratio.end()));

			delete temp_simulator_2;
			delete temp_table_2;
		}
}

void MainWindow::fill_consumer_data(QVector<double>& vector_number_consumers, QVector<double>& vector_prob_reject, QVector<double>& vector_avg_time, QVector<double>& vector_usg_ratio)
{
	Simulator* temp_simulator_3;
	PivotTable* temp_table_3;

  for (int i = 1; i <= vector_number_consumers.size(); i++)
  {
		temp_simulator_3 = new Simulator(4000, 4, 4, i, 3.0);
		temp_simulator_3->run_full_simulation();
		temp_table_3 = temp_simulator_3->get_pivot_table();

    vector_number_consumers[i - 1] = i;
    vector_prob_reject.push_back(*std::max_element(temp_table_3->probability_rejection.begin(), temp_table_3->probability_rejection.end()));
    vector_avg_time.push_back(*std::max_element(temp_table_3->average_elapsed_time.begin(), temp_table_3->average_elapsed_time.end()));
    vector_usg_ratio.push_back(*std::min_element(temp_table_3->usage_ratio.begin(), temp_table_3->usage_ratio.end()));

		delete temp_simulator_3;
		delete temp_table_3;
	}
}

void MainWindow::on_clear_sim_button_clicked()
{
  if (simulator->get_releasing_consumer_time() > 0)
  {
    ui->start_manual_mode_button->setEnabled(true);
    ui->start_manual_mode_button->setText("Сделать итерацию");

    delete simulator;
    delete wg;

    simulator = new Simulator(ui->number_requests->value(), ui->number_producers->value(), ui->buffer_size->value(), ui->number_consumers->value(), ui->lamb->value());
    wg = new WaveformGenerator(ui->number_producers->value(), ui->buffer_size->value(), ui->number_consumers->value(), simulator->get_step_status());

    ui->graphicsView->setScene(wg->get_plot());
  }
}
