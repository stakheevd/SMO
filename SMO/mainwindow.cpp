#include "mainwindow.h"

#include <QString>

#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  //QTableWidget *table = ui->tableWidget;
  //table->setColumnCount(6);
  //table->setColumnWidth(0, 180);
  //table->setColumnWidth(1, 180);
  //table->setColumnWidth(2, 180);
  //table->setColumnWidth(3, 180);
  //table->setColumnWidth(4, 180);
  //table->setColumnWidth(5, 220);
  //QStringList headerList;
  //headerList << "Количество заявок" << "Вероятность отказа" << "Время в системе"
  //           << "Время в буфере" << "Дисперсия буфера" << "Дисперсия обслуживания";
  //table->setHorizontalHeaderLabels(headerList);

  //tables[0]->setRowCount(4);
  //tables[0]->setParent(nullptr);

  //tables[0]->setColumnCount(7);
  //tables[0]->setRowCount(4);
  //tables[0]->setParent(nullptr);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_saveConf_clicked()
{
   ui->statusbar->showMessage("Конфигурация сохранена");
   ui->tabWidget->widget(0)->setDisabled(true);
   ui->tabWidget->widget(1)->setEnabled(true);
   ui->tabWidget->widget(2)->setEnabled(true);

  int number_requests = ui->number_requests->value();
  int number_producers = ui->number_producers->value();
  int number_consumers = ui->number_consumers->value();
  int number_buffers = ui->buffer_size->value();
  double lambda = ui->lamb->value();

  simulator = new Simulator(number_requests, number_producers, number_buffers, number_consumers, lambda);

  wg = new WaveformGenerator(number_producers, number_buffers, number_consumers, simulator->get_status());

  ui->graphicsView->setScene(wg->get_plot());
}

void MainWindow::on_startAutoMode_clicked()
{
   ui->statusbar->setVisible(false);
   ui->startAutoMode->setVisible(false);
   start_simulation();
}

void MainWindow::on_startManualMode_clicked()
{
  ui->statusbar->setVisible(false);
  simulator->take_step();
  wg->take_step(simulator->get_status());
}

void MainWindow::start_simulation()
{
  if (simulator->getConsumer_time() > 0)
  {
    delete simulator;
    // Проверить на утечку
    simulator = new Simulator(ui->number_requests->value(), ui->number_producers->value(), ui->buffer_size->value(), ui->number_consumers->value(), ui->lamb->value());
  }

  // ЗДЕСЬ ПОСТРОЕНИЕ ГРАФИКОВ ДЛЯ РАЗНЫХ ЗНАЧЕНИЙ
  ui->startManualMode->setEnabled(false);
  ui->startManualMode->setText("Симуляция завершена");

  QVector<double> vec_num_producers(80);
  QVector<double> vec_prob_rej_by_prod;
  QVector<double> vec_avg_time_by_prod;
  QVector<double> vec_usage_produces_by_prod;

  //vec_num_producers.reserve(10);
  //vec_prob_rej_by_prod.reserve(10);
  //vec_avg_time_by_prod.reserve(10);
  //vec_usage_produces_by_prod.reserve(10);


  std::thread prod_data([&]()
  {
    fill_producer_data(vec_num_producers, vec_prob_rej_by_prod, vec_avg_time_by_prod, vec_usage_produces_by_prod);
  });

  //// Для разного буфера

  QVector<double> vec_num_buffers(80);
  QVector<double> vec_prob_rej_by_buffers;
  QVector<double> vec_avg_time_by_buffers;
  QVector<double> vec_usage_produces_by_buffers;

  //vec_num_buffers.reserve(10);
  //vec_prob_rej_by_buffers.reserve(10);
  //vec_avg_time_by_buffers.reserve(10);
  //vec_usage_produces_by_buffers.reserve(10);

  std::thread buf_data([&]()
  {
    fill_buffer_data(vec_num_buffers, vec_prob_rej_by_buffers, vec_avg_time_by_buffers, vec_usage_produces_by_buffers);
  });

  // Для разного кол-во приборов

  QVector<double> vec_num_consumers(80);
  QVector<double> vec_prob_rej_by_consumers;
  QVector<double> vec_avg_time_by_consumers;
  QVector<double> vec_usage_produces_by_consumers;

  //vec_num_consumers.reserve(10);
  //vec_prob_rej_by_consumers.reserve(10);
  //vec_avg_time_by_consumers.reserve(10);
  //vec_usage_produces_by_consumers.reserve(10);

  std::thread cons_data([&]()
  {
    fill_consumer_data(vec_num_consumers, vec_prob_rej_by_consumers, vec_avg_time_by_consumers, vec_usage_produces_by_consumers);
  });


  prod_data.join();
  buf_data.join();
  cons_data.join();
  // Здесь начинаем строить графики

  QCustomPlot* plot= ui->prob_by_prod;
  QCPCurve* line= new QCPCurve(plot ->xAxis, plot ->yAxis);
  //plot->xAxis->setBasePen(QPen(Qt::green)); // Красит оси, а не линии графика
  //plot->yAxis->setBasePen(QPen(Qt::green));
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

  // ЭТО ВСЁ ДЛЯ ЗАДАННЫХ ВХОДНЫХ ПАРАМЕТРОВ
  simulator->run_full_simulation();
  PivotTable* table = simulator->get_pivot_table();

  QTableWidget* main_table = ui->tableWidget;
  QTableWidget* us_table = ui->tableWidget_2;

  main_table->setRowCount(simulator->getProducers()->getProducers().size());
  us_table->setRowCount(simulator->getConsumers()->get_consumers().size());

  for (size_t i = 0; i < simulator->getProducers()->getProducers().size(); i++)
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

  for (size_t i = 0; i < simulator->getConsumers()->get_consumers().size(); i++)
  {
    us_table->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
    us_table->setItem(i, 1, new QTableWidgetItem(QString::number(table->usage_ratio[i])));
  }
}

void MainWindow::fill_producer_data(QVector<double> &v_n_prod, QVector<double> &v_p_r, QVector<double> &v_a_t, QVector<double> &v_u_p)
{
	Simulator* temp_simulator_1;
	PivotTable* temp_table_1;

	for (int i = 1; i <= v_n_prod.size(); i++)
	{ // Поменять входные параметры
		temp_simulator_1 = new Simulator(4000, i, 4, 4, 3.0);
		temp_simulator_1->run_full_simulation();
		temp_table_1 = temp_simulator_1->get_pivot_table();

		v_n_prod[i - 1] = i;
		v_p_r.push_back(*std::max_element(temp_table_1 ->probability_rejection.begin(), temp_table_1->probability_rejection.end()));
		v_a_t.push_back(*std::max_element(temp_table_1->average_elapsed_time.begin(), temp_table_1->average_elapsed_time.end()));
		v_u_p.push_back(*std::min_element(temp_table_1->usage_ratio.begin(), temp_table_1->usage_ratio.end()));

		delete temp_simulator_1;
		delete temp_table_1;
	}
}

void MainWindow::fill_buffer_data(QVector<double> &v_n_bufs, QVector<double> &v_p_r, QVector<double> &v_a_t, QVector<double> &v_u_p)
{
		Simulator* temp_simulator_2;
		PivotTable* temp_table_2;

    for (int i = 1; i <= v_n_bufs.size(); i++)
		{ // Поменять входные параметры
			temp_simulator_2 = new Simulator(4000, 4, i, 4, 3.0);
			temp_simulator_2->run_full_simulation();
			temp_table_2 = temp_simulator_2->get_pivot_table();

			v_n_bufs[i - 1] = i;
			v_p_r.push_back(*std::max_element(temp_table_2->probability_rejection.begin(), temp_table_2->probability_rejection.end()));
			v_a_t.push_back(*std::max_element(temp_table_2->average_elapsed_time.begin(), temp_table_2->average_elapsed_time.end()));
			v_u_p.push_back(*std::min_element(temp_table_2->usage_ratio.begin(), temp_table_2->usage_ratio.end()));

			delete temp_simulator_2;
			delete temp_table_2;
		}
}

void MainWindow::fill_consumer_data(QVector<double> &v_n_cons, QVector<double> &v_p_r, QVector<double> &v_a_t, QVector<double> &v_u_p)
{
	Simulator* temp_simulator_3;
	PivotTable* temp_table_3;

	for (int i = 1; i <= v_n_cons.size(); i++)
	{ // Поменять входные параметры
		temp_simulator_3 = new Simulator(4000, 4, 4, i, 3.0);
		temp_simulator_3->run_full_simulation();
		temp_table_3 = temp_simulator_3->get_pivot_table();

		v_n_cons[i - 1] = i;
		v_p_r.push_back(*std::max_element(temp_table_3->probability_rejection.begin(), temp_table_3->probability_rejection.end()));
		v_a_t.push_back(*std::max_element(temp_table_3->average_elapsed_time.begin(), temp_table_3->average_elapsed_time.end()));
		v_u_p.push_back(*std::min_element(temp_table_3->usage_ratio.begin(), temp_table_3->usage_ratio.end()));

		delete temp_simulator_3;
		delete temp_table_3;
	}
}

void MainWindow::on_clear_sim_button_clicked()
{
  if (simulator->getConsumer_time() > 0)
  {
    ui->startManualMode->setEnabled(true);
    ui->startManualMode->setText("Сделать итерацию");

    delete simulator;
    delete wg;

    int number_requests = ui->number_requests->value();
    int number_producers = ui->number_producers->value();
    int number_consumers = ui->number_consumers->value();
    int number_buffers = ui->buffer_size->value();
    double lambda = ui->lamb->value();

    simulator = new Simulator(number_requests, number_producers, number_buffers, number_consumers, lambda);

    wg = new WaveformGenerator(number_producers, number_buffers, number_consumers, simulator->get_status());

    ui->graphicsView->setScene(wg->get_plot());
  }
  else
  {
    // TODO: SMB
  }
}

