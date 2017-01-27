#include "intervalometer.hpp"

Intervalometer::Intervalometer(QObject *parent)
		:QObject(parent)
		,counter_(0)
		,exposure_(0)
{
	elapsed_.invalidate();
	connect(&camera_, &Camera::finished, [this]() {
		counter_ -= 1;
		if (counter_ > 0) {
			between_.start();
			elapsed_.start();
		}
		else emit finished();
	});
	between_.setSingleShot(true);
	connect(&between_, &QTimer::timeout, [this]() {
		elapsed_.invalidate();
		camera_.shoot(exposure_);
	});
	initial_.setSingleShot(true);
	connect(&initial_, &QTimer::timeout, [this]() {
		elapsed_.start();
		camera_.shoot(exposure_);
	});
}



QString Intervalometer::status() const
{
	QString result;
	if (elapsed_.isValid()) {
		result = QString("Esperando %1 segundos, quedan %2 fotos").arg(elapsed_.elapsed() / 1000).arg(counter_);
	} else {
		result = QString("Exponiendo %1 segundos, quedan %2 fotos").arg(camera_.shooting() / 1000).arg(counter_);
	}
	return result;
}



void Intervalometer::start(int counter, int exposure, int interval_delay, int initial_delay)
{
	if (interval_delay < MIN_DELAY) interval_delay = MIN_DELAY;
	if (initial_delay < MIN_DELAY) initial_delay = MIN_DELAY;
	cancel();
	between_.setInterval(interval_delay);
	initial_.setInterval(initial_delay);
	if (counter >= 1) {
		counter_ = counter;
		exposure_ = exposure;
		initial_.start();
	}
}



void Intervalometer::cancel()
{
	camera_.cancel();
	between_.stop();
	initial_.stop();
	counter_ = 0;
	exposure_ = 0;
}
