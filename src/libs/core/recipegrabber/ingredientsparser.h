#ifndef INGRIDIENTSPARSER_H
#define INGRIDIENTSPARSER_H
#include <QString>
#include <QStringList>
#include <vector>
#include <core/db/ingredient.h>
#include <map>

namespace Mamatkon {

class IngredientsParser {
	typedef ::std::vector<QString> str_list;

	class ComponentExtracor {
	public:
		QString extractAndRemoveFrom(QStringList& words);

	protected:
		QString buildStringFromRange(const QStringList& words, int start, int end);
		str_list findItemsThatStartsWith(const str_list& list,  const QString& word);
		QString removePunctuationMarksFrom(const QString& word);
		bool twoItemsAreSemanticallyTheSame(const QString& item1, const QString& item2);
		void findRangeInList(const std::vector<QString>& list, QStringList &words, int start, int& end );

		virtual void findMatchedRange(QStringList& words, int start, int& end) = 0;
	};

	class AmountExtractor : public ComponentExtracor {
		std::map<QString, QString>& m_elements;

		void findAmountInNumbers(QStringList& words, int start, int& end);
		void findAmountInWords(QStringList& words, int start, int& end);
		void compileNextNumber(QStringList &words, int sStart, int& end );
		str_list createKeyListFrom(std::map<QString, QString> elementsMap);
		QString findValueFor(const QString& word) const;
	public:
		AmountExtractor(std::map<QString, QString>& elements) : m_elements(elements) {}
	protected:
		void findMatchedRange(QStringList& words, int start, int& end);
	};

	class FromListExtractor : public ComponentExtracor {
		str_list& m_elements;
	public:
		FromListExtractor(str_list& elements) : m_elements(elements) {}
	protected:
		void findMatchedRange(QStringList& words, int start, int& end);
	};

	class MethodExtractor : public ComponentExtracor {
	protected:
		void findMatchedRange(QStringList& words, int start, int& end);
	};

	str_list m_products;
	str_list m_measures;
	std::map<QString, QString> m_amounts;

	Ingredient createIngridientFrom(const QString& text);
	void fillMisingIngridientInfo(Ingredient& ingridient, const Ingredient& origIngridient);
	void concatStringLists(str_list& targetList, const str_list& sourceList);
	QStringList splitByAlternateIngridients(const QString& row);
public:
	::std::vector<Ingredient> parseIngridients(const QString& text);

	void setProducts(const std::vector<QString>& products) { m_products = products; }
	void setMeasures(const std::vector<QString>& measures) { m_measures = measures; }
	void setAmounts(const std::map<QString, QString>& amounts) { m_amounts =  amounts; }
};
}
#endif // INGRIDIENTSPARSER_H
