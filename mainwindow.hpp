#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>
#include "intervalometer.hpp"

class TouchSpin;
class QPushButton;
class QLabel;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);

private:
	TouchSpin* exposure_;
	TouchSpin* count_;
	TouchSpin* delay_;
	QPushButton* start_cancel_;
	QLabel* status_;
	QTimer periodic_;
	Intervalometer intervalometer_;
};

#endif // MAINWINDOW_HPP
