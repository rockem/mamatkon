#include "numberscaler.h"
#include <QRegExp>
#include <QStringList>
#include <boost/math/common_factor.hpp>

namespace Mamatkon {

using ::boost::math::gcd;

QString NumberScaler::getScaledNumber( const QString& number ) {
	QString scaledNumber;
	scaledNumber = NumberAndFractionScalerTemplate(m_ratio).scale(number);
	if(!scaledNumber.isEmpty()) {
		return scaledNumber;
	}
	scaledNumber = FractionScalerTemplate(m_ratio).scale(number);
	if(!scaledNumber.isEmpty()) {
		return scaledNumber;
	}
	scaledNumber = NumberScalerTemplate(m_ratio).scale(number);
	if(!scaledNumber.isEmpty()) {
		return scaledNumber;
	}

	return number;
}

QString NumberScaler::ScalerTemplate::scaleNumber(const QString& number ) {
	QString scaledNumber;
	int numer = extractNumer(number) * extractNumer(m_ratio);
	int denum = extractDenum(number) * extractDenum(m_ratio);

	int commonFactor = gcd<int>( numer,  denum);
	numer = numer / commonFactor;
	denum = denum / commonFactor;

	if(denum == 1) {
		return QString::number(numer);
	}

	if(numer == denum) {
		return "1";
	}

	if(numer > denum) {
		int num = numer / denum;
		numer = numer % denum;
		scaledNumber = QString::number(num) + " ";
	}

	scaledNumber += QString::number(numer) + "/" + QString::number(denum);
	return scaledNumber;
}

int NumberScaler::ScalerTemplate::extractNumer( const QString &number ) const {
	return (number.split("/")[0]).toInt();
}

int NumberScaler::ScalerTemplate::extractDenum( const QString &number ) const {
	return (number.split("/")[1]).toInt();
}

QString NumberScaler::ScalerTemplate::scale( const QString& number ) {
	QRegExp rx(getPattern());
	if(rx.exactMatch(number)) {
		return scaleNumber(converToSimulateFraction(number));
	}
	return "";
}

QString NumberScaler::NumberAndFractionScalerTemplate::converToSimulateFraction(const QString& number) const {
	QStringList numbers = number.split(" ");
	int denominator = extractDenum(numbers[1]);
	int numerator = extractNumer(numbers[1]);
	return QString::number(denominator * numbers[0].toInt() + numerator) + "/" + QString::number(denominator);
}

}