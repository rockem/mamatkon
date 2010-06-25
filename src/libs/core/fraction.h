#ifndef FRACTION_H
#define FRACTION_H
#include <QString>

namespace Mamatkon { 

	class Fraction {

		struct SFraction {
			int _Numerator;
			int _Denumerator;

			SFraction() : _Numerator(0), _Denumerator(0) {}
			SFraction(int numer, int denum) : _Numerator(numer), _Denumerator(denum) {}
		};

		class StrToFractionConverter {
			int& numerator;
			int& denumerator;

		public:
			void convertIfCompatible(const QString& number);
		protected:
			StrToFractionConverter(int& numer, int& denum) : numerator(numer), denumerator(denum) {}
			int extractNumer(const QString &number) const;
			int extractDenum(const QString &number) const;
			SFraction createSFraction(int numer, int denum);

			virtual QString getPattern() const = 0;
			virtual SFraction converToSimulateFraction(const QString& number) const = 0;
		};

		class NumberAndFractioneConverter : public StrToFractionConverter {
		public :
			NumberAndFractioneConverter(int& numer, int& denum) : StrToFractionConverter(numer, denum) {}
		protected:
			QString getPattern() const { return "(\\d+ \\d+/\\d+)"; }
			SFraction converToSimulateFraction(const QString& number) const;
		};

		class FractionConverter : public StrToFractionConverter {
		public :
			FractionConverter(int& numer, int& denum) : StrToFractionConverter(numer, denum) {}
		protected:
			QString getPattern() const { return "(\\d+/\\d+)"; }
			SFraction converToSimulateFraction(const QString& number) const;
		};

		class NumberConverter : public StrToFractionConverter {
		public :
			NumberConverter(int& numer, int& denum) : StrToFractionConverter(numer, denum) {}
		protected:
			QString getPattern() const { return "(\\d+)"; }
			SFraction converToSimulateFraction(const QString& number) const;
		};

		int numer;
		int denum;
	
	public:
		Fraction(const QString& str);

		float toFloat() const;
	};

}
#endif // FRACTION_H
