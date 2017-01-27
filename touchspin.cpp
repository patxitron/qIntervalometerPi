#include "touchspin.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QList>



TouchSpinDigit::TouchSpinDigit(quint8 value, bool overflow_enabled, QWidget* parent)
		:QWidget(parent)
		,value_(value)
		,overflow_enabled_(overflow_enabled)
		,up_(new QPushButton(QIcon(":/icons/up.png"), ""))
		,down_(new QPushButton(QIcon(":/icons/down.png"), ""))
		,label_(new QLabel())
{
	setStyleSheet("QLabel {"
	                  "background-color: black;"
	                  "color: red;"
	                  "border-style: solid;"
	                  "border-width: 2px;"
	                  "border-radius: 3px;"
	                  "border-color: red;"
	                  "font-size: 48px;"
	                  "margin: 0px;"
	              "} "
	              "QLabel:disabled {"
	                  "color: #440000;"
	                  "border-color: #440000"
	              "} "
	              "QPushButton {"
	                  "background-color: black;"
	                  "color: red;"
	                  "margin: 0px;"
	                  "padding-top: 8px;"
	                  "padding-bottom: 8px;"
	              "} "
	              "QPushButton:disabled {"
	                  "color: #440000;"
	                  "border-color: #440000;"
	                  "background-image: url(':/icons/null.png');"
	              "} "
	              "QWidget:disabled {"
	                  "color: #440000;"
	              "}");
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(up_);
	layout->addWidget(label_);
	layout->addWidget(down_);
	label_->setText(QString("%1").arg(value_));
	setLayout(layout);
	connect(up_, &QPushButton::clicked, this, &TouchSpinDigit::increment);
	connect(down_, &QPushButton::clicked, this, &TouchSpinDigit::decrement);
}



void TouchSpinDigit::setValue(quint8 value)
{
	if (value > 9) value = 9;
	value_ = value;
	label_->setText(QString("%1").arg(value_));
	emit valueChanged(value_);
}



void TouchSpinDigit::increment()
{
	if (value_ < 9) {
		value_ += 1;
		label_->setText(QString("%1").arg(value_));
		emit valueChanged(value_);
	} else {
		if (overflow_enabled_) {
			value_ = 0;
			label_->setText(QString("%1").arg(value_));
			emit overflow();
			emit valueChanged(value_);
		}
	}
}



void TouchSpinDigit::decrement()
{
	if (value_ > 0) {
		value_ -= 1;
		label_->setText(QString("%1").arg(value_));
		emit valueChanged(value_);
	} else {
		if (overflow_enabled_) {
			value_ = 9;
			label_->setText(QString("%1").arg(value_));
			emit underflow();
			emit valueChanged(value_);
		}
	}
}






TouchSpin::TouchSpin(QString const& label, quint8 digits, quint32 value
                    ,bool overflow_enabled, QWidget* parent)
		:QWidget(parent)
		,overflow_enabled_(overflow_enabled)
{
	if (digits > MAX_DIGITS) digits = MAX_DIGITS;
	QList<quint8> decimaldigits;
	while(value) {
		decimaldigits.append(static_cast<quint8>(value % 10));
		value /= 10;
	}
	if (decimaldigits.size() < digits) {
		for (size_t i = decimaldigits.size(); i < digits; i++) {
			decimaldigits.append(0);
		}
	} else if (decimaldigits.size() > digits) {
		decimaldigits.clear();
		for (size_t i = 0; i < digits; i++) {
			decimaldigits.append(9);
		}
	}
	digits_.reserve(digits);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(new QLabel(label));
	QHBoxLayout* container = new QHBoxLayout;
	for (size_t i = 0; i < digits; i++) {
		TouchSpinDigit* digit = new TouchSpinDigit(decimaldigits[digits - 1 - i], true); // TODO: value está mal, hay que seleccionar el dígito
		digits_.append(digit);
		container->addWidget(digit);
		connect(digit, &TouchSpinDigit::overflow, this, &TouchSpin::digitOverflow);
		connect(digit, &TouchSpinDigit::underflow, this, &TouchSpin::digitUnderflow);
	}
	layout->addLayout(container);
	setLayout(layout);
}



quint32 TouchSpin::value() const
{
	quint32 result = 0;
	for (int i = 0; i < digits_.size(); i++) {
		result *= 10;
		result += digits_[i]->value();
	}
	return result;
}



void TouchSpin::setValue(quint32 value)
{
	quint8 digits = digits_.size();
	QList<quint8> decimaldigits;
	while(value) {
		decimaldigits.append(static_cast<quint8>(value % 10));
		value /= 10;
	}
	if (decimaldigits.size() < digits) {
		for (size_t i = decimaldigits.size(); i < digits; i++) {
			decimaldigits.append(0);
		}
	} else if (decimaldigits.size() > digits) {
		decimaldigits.clear();
		for (size_t i = 0; i < digits; i++) {
			decimaldigits.append(9);
		}
	}
	for (size_t i = 0; i < digits; i++) {
		digits_[i]->setValue(decimaldigits[digits - 1 - i]);
	}
}



void TouchSpin::increment()
{
	digits_[digits_.size() - 1]->increment();
}



void TouchSpin::decrement()
{
	digits_[digits_.size() - 1]->decrement();
}



void TouchSpin::digitOverflow()
{
	int index = digits_.indexOf(reinterpret_cast<TouchSpinDigit*>(sender()));
	if (index > 0) digits_[index - 1]->increment();
}



void TouchSpin::digitUnderflow()
{
	int index = digits_.indexOf(reinterpret_cast<TouchSpinDigit*>(sender()));
	if (index > 0) digits_[index - 1]->decrement();
}
