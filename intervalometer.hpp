#ifndef INTERVALOMETER_HPP
#define INTERVALOMETER_HPP

#include <QObject>
#include <QTimer>
#include <QString>
#include <QElapsedTimer>
#include "camera.hpp"

class Intervalometer : public QObject
{
	Q_OBJECT
public:
	static int const MIN_DELAY = 2000; // 2 seconds

	explicit Intervalometer(QObject *parent = 0);

	Camera& camera() {return camera_;}
	Camera const& camera() const {return camera_;}

	QString status() const;

signals:
	void finished();

public slots:
	void start(int counter, int exposure, int interval_delay=MIN_DELAY, int initial_delay=MIN_DELAY);
	void cancel();

private:
	int counter_;
	int exposure_;
	QElapsedTimer elapsed_;
	Camera camera_;
	QTimer between_;
	QTimer initial_;
};

#endif // INTERVALOMETER_HPP
