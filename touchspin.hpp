#ifndef TOUCHSPIN_HPP
#define TOUCHSPIN_HPP

#include <QWidget>
#include <QList>

class QPushButton;
class QLabel;


class TouchSpinDigit : public QWidget
{
	Q_OBJECT
public:
	explicit TouchSpinDigit(quint8 value=0, bool overflow_enabled=false
	                       ,QWidget *parent=0);

	quint8 value() const {return value_;}
	bool overflowEnabled() const {return overflow_enabled_;}
	void setOverflowEnabled(bool enabled=true) {overflow_enabled_ = enabled;}
	void setOverflowDisabled() {overflow_enabled_ = false;}

signals:
	void valueChanged(quint8);
	void overflow();
	void underflow();

public slots:
	void setValue(quint8 value);
	void increment();
	void decrement();

private:
	quint8 value_;
	bool overflow_enabled_;
	QPushButton* up_;
	QPushButton* down_;
	QLabel* label_;
};



class TouchSpin: public QWidget
{
	Q_OBJECT
public:
	static size_t const MAX_DIGITS = 6;

	explicit TouchSpin(QString const& label, quint8 digits, quint32 value=0
	                  ,bool overflow_enabled=false, QWidget *parent=0);

	quint32 value() const;
	bool overflowEnabled() const {return overflow_enabled_;}
	void setOverflowEnabled(bool enabled=true) {overflow_enabled_ = enabled;}
	void setOverflowDisabled() {overflow_enabled_ = false;}

signals:
	void valueChanged(quint32);
	void overflow();
	void underflow();

public slots:
	void setValue(quint32 value);
	void increment();
	void decrement();

private slots:
	void digitOverflow();
	void digitUnderflow();

private:
	bool overflow_enabled_;
	QList<TouchSpinDigit*> digits_;
};

#endif // TOUCHSPIN_HPP
