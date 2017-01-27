#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <QObject>
#include <QString>
#include <QFile>
#include <QTimer>
#include <QElapsedTimer>
#include <stdexcept>



struct CameraException: public std::runtime_error
{
	CameraException(QString const& what): std::runtime_error(what.toStdString()) {}
};



class Camera : public QObject
{
	Q_OBJECT
public:
	static int const FOCUS = 22; // Focus GPIO
	static int const SHOOT = 23; // Shoot GPIO
	static int const PREFOCUS_TIME = 200; // Miliseconds
	static int const MAX_EXPOSURE = 480000; // 8 minutes
	static int const MIN_EXPOSURE = 2000; // 2 seconds

	explicit Camera(QObject *parent = 0);
	~Camera();

	qint64 shooting() const;

signals:
	void finished();

public slots:
	void shoot(int exposure);
	void cancel();

private slots:
	void pressFocus();
	void pressShoot();
	void releaseFocus();
	void releaseShoot();

private:
	QFile focus_out_;
	QFile shoot_out_;
	QTimer pre_timer_;
	QTimer shoot_timer_;
	QTimer post_timer_;
	QElapsedTimer elapsed_;
};

#endif // CAMERA_HPP
