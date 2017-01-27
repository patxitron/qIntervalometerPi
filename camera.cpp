#include "camera.hpp"
#include <cstring>
#include <errno.h>


Camera::Camera(QObject *parent)
		:QObject(parent)
		,focus_out_(QString("/sys/class/gpio/gpio%1/value").arg(FOCUS))
		,shoot_out_(QString("/sys/class/gpio/gpio%1/value").arg(SHOOT))
{
	QFile gpio_ctl("/sys/class/gpio/export");
	if (!gpio_ctl.open(QIODevice::WriteOnly)) {
		throw CameraException(QString("Cannot open '%1': %2").arg(gpio_ctl.fileName()).arg(std::strerror(errno)));
	}
	gpio_ctl.write(QString("%1").arg(FOCUS).toLocal8Bit());
	gpio_ctl.close();
	if (!gpio_ctl.open(QIODevice::WriteOnly)) {
		throw CameraException(QString("Cannot open '%1': %2").arg(gpio_ctl.fileName()).arg(std::strerror(errno)));
	}
	gpio_ctl.write(QString("%1").arg(SHOOT).toLocal8Bit());
	gpio_ctl.close();
	gpio_ctl.setFileName(QString("/sys/class/gpio/gpio%1/direction").arg(FOCUS));
	if (!gpio_ctl.open(QIODevice::WriteOnly)) {
		throw CameraException(QString("Cannot open '%1': %2").arg(gpio_ctl.fileName()).arg(std::strerror(errno)));
	}
	gpio_ctl.write("out");
	gpio_ctl.close();
	gpio_ctl.setFileName(QString("/sys/class/gpio/gpio%1/direction").arg(SHOOT));
	if (!gpio_ctl.open(QIODevice::WriteOnly)) {
		throw CameraException(QString("Cannot open '%1': %2").arg(gpio_ctl.fileName()).arg(std::strerror(errno)));
	}
	gpio_ctl.write("out");
	gpio_ctl.close();
	elapsed_.invalidate();
	pre_timer_.setInterval(PREFOCUS_TIME);
	pre_timer_.setSingleShot(true);
	connect(&pre_timer_, &QTimer::timeout, this, &Camera::pressShoot);
	post_timer_.setInterval(PREFOCUS_TIME);
	post_timer_.setSingleShot(true);
	connect(&post_timer_, &QTimer::timeout, this, &Camera::releaseFocus);
	shoot_timer_.setSingleShot(true);
	connect(&shoot_timer_, &QTimer::timeout, this, &Camera::releaseShoot);
}



Camera::~Camera()
{
	cancel();
	QFile gpio_ctl("/sys/class/gpio/unexport");
	if (gpio_ctl.open(QIODevice::WriteOnly)) {
		gpio_ctl.write(QString("%1").arg(FOCUS).toLocal8Bit());
		gpio_ctl.close();
	}
	if (gpio_ctl.open(QIODevice::WriteOnly)) {
		gpio_ctl.write(QString("%1").arg(SHOOT).toLocal8Bit());
	}
}



qint64 Camera::shooting() const
{
	if (elapsed_.isValid()) {
		return elapsed_.elapsed();
	}
	return 0;
}



void Camera::shoot(int exposure)
{
	if (exposure < MIN_EXPOSURE) exposure = MIN_EXPOSURE;
	if (exposure > MAX_EXPOSURE) exposure = MAX_EXPOSURE;
	shoot_timer_.setInterval(exposure);
	pressFocus();
}



void Camera::cancel()
{
	pre_timer_.stop();
	post_timer_.stop();
	shoot_timer_.stop();
	releaseShoot();
}



void Camera::pressFocus()
{
	if (!focus_out_.open(QIODevice::WriteOnly)) {
		throw CameraException(QString("Cannot open '%1': %2").arg(focus_out_.fileName()).arg(std::strerror(errno)));
	}
	focus_out_.write("1");
	focus_out_.close();
	elapsed_.start();
	pre_timer_.start();
}



void Camera::pressShoot()
{
	if (!shoot_out_.open(QIODevice::WriteOnly)) {
		throw CameraException(QString("Cannot open '%1': %2").arg(shoot_out_.fileName()).arg(std::strerror(errno)));
	}
	shoot_out_.write("1");
	shoot_out_.close();
	shoot_timer_.start();
}



void Camera::releaseFocus()
{
	if (!focus_out_.open(QIODevice::WriteOnly)) {
		throw CameraException(QString("Cannot open '%1': %2").arg(focus_out_.fileName()).arg(std::strerror(errno)));
	}
	focus_out_.write("0");
	focus_out_.close();
	elapsed_.invalidate();
	emit finished();
}



void Camera::releaseShoot()
{
	if (!shoot_out_.open(QIODevice::WriteOnly)) {
		throw CameraException(QString("Cannot open '%1': %2").arg(shoot_out_.fileName()).arg(std::strerror(errno)));
	}
	shoot_out_.write("0");
	shoot_out_.close();
	post_timer_.start();
}
