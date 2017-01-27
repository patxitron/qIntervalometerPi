#include "mainwindow.hpp"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "touchspin.hpp"

MainWindow::MainWindow(QWidget *parent)
		:QMainWindow(parent)
		,exposure_(new TouchSpin("Exposición", 3, 2, true))
		,count_(new TouchSpin("Número", 3, 1, true))
		,delay_(new TouchSpin("Espera", 2, 2, true))
		,start_cancel_(new QPushButton("COMENZAR"))
		,status_(new QLabel("En espera"))
{
	setStyleSheet("QLabel {"
	                  "background-color: black;"
	                  "color: red;"
	                  "text-align: center;"
	                  "font-size: 24px;"
	              "} "
	              "QPushButton {"
	                  "background-color: black;"
	                  "color: red;"
	                  "font: bold 48px;"
	                  "border-style: solid;"
	                  "border-width: 2px;"
	                  "border-radius: 10px;"
	                  "border-color: red;"
	              "} "
	              "QWidget {"
	                  "background-color: black;"
	                  "color: red;"
	              "}");
	QWidget* main_widget = new QWidget;
	QVBoxLayout* main_layout = new QVBoxLayout;
	QHBoxLayout* spins = new QHBoxLayout;
	spins->addWidget(exposure_);
	spins->addWidget(count_);
	spins->addWidget(delay_);
	main_layout->addLayout(spins);
	main_layout->addWidget(start_cancel_);
	main_layout->addWidget(status_);
	main_widget->setLayout(main_layout);
	setCentralWidget(main_widget);
	periodic_.setSingleShot(false);
	periodic_.setInterval(100);
	connect(&periodic_, &QTimer::timeout, [this]() {
		status_->setText(intervalometer_.status());
	});
	connect(&intervalometer_, &Intervalometer::finished, [this]() {
		start_cancel_->setText("COMENZAR");
		periodic_.stop();
		status_->setText("Terminado");
		exposure_->setDisabled(false);
		count_->setDisabled(false);
		delay_->setDisabled(false);
	});
	connect(start_cancel_, &QPushButton::clicked, [this]() {
		if (start_cancel_->text() == "COMENZAR") {
			start_cancel_->setText("PARAR");
			exposure_->setDisabled(true);
			count_->setDisabled(true);
			delay_->setDisabled(true);
			intervalometer_.start(count_->value(), exposure_->value() * 1000, delay_->value(), delay_->value());
			periodic_.start();
		} else {
			start_cancel_->setText("COMENZAR");
			periodic_.stop();
			status_->setText("Cancelado");
			exposure_->setDisabled(false);
			count_->setDisabled(false);
			delay_->setDisabled(false);
			intervalometer_.cancel();
		}
	});
}
