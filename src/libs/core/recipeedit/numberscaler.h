#ifndef NUMBERSCALER_H
#define NUMBERSCALER_H
#include <QString>

namespace Mamatkon { 

class NumberScaler {

	class ScalerTemplate {
		const QString& m_ratio;

		QString scaleNumber(const QString& number);
	public:
		ScalerTemplate(const QString& ratio) : m_ratio(ratio) {}
		QString scale(const QString& number);
	protected:
		int extractNumer(const QString &number) const;
		int extractDenum(const QString &number) const;

		virtual QString getPattern() const = 0;
		virtual QString converToSimulateFraction(const QString& number) const = 0;
	};

	class NumberAndFractionScalerTemplate : public ScalerTemplate {
	public:
		NumberAndFractionScalerTemplate(const QString& ratio) : ScalerTemplate(ratio) {}
	protected:
		QString getPattern() const { return "(\\d+ \\d+/\\d+)"; }
		QString converToSimulateFraction(const QString& number) const;
	};

	class FractionScalerTemplate : public ScalerTemplate {
	public:
		FractionScalerTemplate(const QString& ratio) : ScalerTemplate(ratio) {}
	protected:
		QString getPattern() const { return "(\\d+/\\d+)"; }
		QString converToSimulateFraction(const QString& number) const { return number; }
	};

	class NumberScalerTemplate : public ScalerTemplate {
	public:
		NumberScalerTemplate(const QString& ratio) : ScalerTemplate(ratio) {}
	protected:
		QString getPattern() const { return "(\\d+)"; }
		QString converToSimulateFraction(const QString& number) const { return number + "/1"; }
	};

	const QString& m_ratio;

public:
	NumberScaler(const QString& ratio) : m_ratio(ratio) {}
	QString getScaledNumber(const QString& number);
};

}
#endif // NUMBERSCALER_H