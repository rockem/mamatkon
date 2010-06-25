#include "fraction.h"
#include <QStringList>
#include <QregExp>

using namespace Mamatkon;

Mamatkon::Fraction::Fraction(const QString& number) : numer(0), denum(0){
	NumberAndFractioneConverter(numer, denum).convertIfCompatible(number);
	FractionConverter(numer, denum).convertIfCompatible(number);
	NumberConverter(numer, denum).convertIfCompatible(number);
}

float Fraction::toFloat() const {
	return numer / (float)denum;
}

int Fraction::
StrToFractionConverter::extractNumer( const QString &number ) const {
	return (number.split("/")[0]).toInt();
}

int Fraction::
StrToFractionConverter::extractDenum( const QString &number ) const {
	return (number.split("/")[1]).toInt();
}

void Fraction::
StrToFractionConverter::convertIfCompatible(const QString& number) {
	QRegExp rx(getPattern());
	if(rx.exactMatch(number)) {
		Fraction::SFraction f =  converToSimulateFraction(number);
		numerator = f._Numerator;
		denumerator = f._Denumerator;
	}
}

Fraction::SFraction Fraction::
NumberAndFractioneConverter::converToSimulateFraction(const QString& number) const {
	QStringList numbers = number.split(" ");
	Fraction::SFraction f;
	f._Denumerator = extractDenum(numbers[1]);
	f._Numerator = f._Denumerator * numbers[0].toInt() + extractNumer(numbers[1]);
	return f;
}

Fraction::SFraction Fraction::
FractionConverter::converToSimulateFraction( const QString& number ) const {
	return Fraction::SFraction(extractNumer(number), extractDenum(number));
}

Fraction::SFraction Fraction::
NumberConverter::converToSimulateFraction( const QString& number ) const {
	return Fraction::SFraction(number.toInt(), 1);
}
