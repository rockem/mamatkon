#include "ingredientsparser.h"
#include <QRegExp>
#include <algorithm>

namespace Mamatkon {

using ::std::vector;
using ::std::map;

::std::vector<Ingredient> IngredientsParser::parseIngridients( const QString& text ) {
	vector<Ingredient> ingridients;
	QStringList lines = text.split("\n", QString::SkipEmptyParts);
	int idCounter = 1;
	for(int i = 0; i < lines.size(); ++i) {
		Ingredient ingridient = createIngridientFrom(lines[i]);
		ingridient._Id = idCounter;
		QStringList options = splitByAlternateIngridients(ingridient._Method);
		ingridient._Method = options[0];
		ingridients.push_back(ingridient);
		for(int j = 1; j < options.size(); ++j) {
			Ingredient altIngridient = createIngridientFrom(options[j]);
			if(altIngridient._ProductId.isEmpty()) {
				ingridients[idCounter - 1]._Method += " / " + altIngridient._Method;
			} else {
				fillMisingIngridientInfo(altIngridient, ingridient);
				idCounter++;
				altIngridient._Id = idCounter;
				ingridients.push_back(altIngridient);
			}
		}
		idCounter++;
	}
	return ingridients;
}

bool stringLengthComprasion(const QString& str1, const QString& str2) {
	return str1.length() < str2.length();
}

QStringList IngredientsParser::splitByAlternateIngridients(const QString& row) {
	QRegExp exp(QString::fromStdWString(L"(\\Wאו\\s|\\s\\/\\s|^/\\s)"));
	int pos = 0;
	int start = 0;
	QStringList options;
	while((pos = exp.indexIn(row, pos)) != -1) {
		QString s = exp.cap();
		int as = s.indexOf(s);
		
		options << row.section("", start, pos + as);
		pos += exp.matchedLength();
		start = pos;
	}
	options << row.section("", start, row.length());
	return options;
}

Ingredient IngredientsParser::createIngridientFrom(const QString &text) {
	QStringList words = text.split(" ", QString::SkipEmptyParts);
	Ingredient ing;
	ing._Amount = AmountExtractor(m_amounts).extractAndRemoveFrom(words);
	ing._MeasureId = FromListExtractor(m_measures).extractAndRemoveFrom(words);
	ing._ProductId = FromListExtractor(m_products).extractAndRemoveFrom(words);
	ing._Method = MethodExtractor().extractAndRemoveFrom(words);
	return ing;
}

void IngredientsParser::fillMisingIngridientInfo(Ingredient& ingridient, const Ingredient& origIngridient) {
	if(ingridient._Amount.isEmpty()) {
		ingridient._Amount = origIngridient._Amount;
	}
	if(ingridient._MeasureId.isEmpty()) {
		ingridient._MeasureId = origIngridient._MeasureId;
	}
	ingridient._AltId = origIngridient._Id;
}

void IngredientsParser::concatStringLists(str_list& targetList, const str_list& sourceList) {
	str_list::iterator i = targetList.begin();
	targetList.insert(i, sourceList.begin(), sourceList.end());
}

QString IngredientsParser::
ComponentExtracor::extractAndRemoveFrom(QStringList& words) {
	int start = 0, end = 0;
	for (; start < words.size(); ++start) {
		findMatchedRange(words, start, end);
		if(start < end) {
			break;
		}
	}
	QString component = buildStringFromRange(words, start, end);
	for (int i = start; i < end; ++i) {
		words.removeAt(start);
	}
	return component;
}

void IngredientsParser::
AmountExtractor::findMatchedRange(QStringList& words, int start, int& end) {
	findAmountInNumbers(words, start, end);
	if(start >= end) {
		findAmountInWords(words, start, end);
		if(words.size() > end + 1 && words[end] == QString::fromStdWString(L"òã")) {
			words[end++] = "-";
			findAmountInWords(words, end, end);
		}
	}
}

void IngredientsParser::
AmountExtractor::findAmountInNumbers(QStringList& words, int start, int& end) {
	QRegExp rx("(\\d+|\\d+/\\d+|\\d+-\\d+)");
	if(rx.exactMatch(words[start])) {
		end = start + 1;
	}

	rx.setPattern("\\d+/\\d+ \\d+");
	if(words.size() > (start + 1) && rx.exactMatch(buildStringFromRange(words, start, start + 2))) {
		end = start + 2;
		words.swap(start, start + 1);
		
	}

	rx.setPattern("\\d+ - \\d+");
	if(words.size() > (start + 2) && rx.exactMatch(buildStringFromRange(words, start, start + 3))) {
		end = start + 3;
	}
}

void IngredientsParser::
AmountExtractor::findAmountInWords(QStringList& words, int start, int& end) {
	compileNextNumber(words, start, end);
	if(start < end) {
		if(words.size() > end && QString(words[end][0]) == QString::fromStdWString(L"å")) {
			words[end].replace(0, 1, "");
			compileNextNumber(words, end, end);
		}
	}
}

IngredientsParser::str_list IngredientsParser::
AmountExtractor::createKeyListFrom(std::map<QString, QString> elementsMap) {
	vector<QString> keyList;
	map<QString, QString>::const_iterator it = elementsMap.begin();
	for(; it != elementsMap.end(); ++it) {
		keyList.push_back(it->first);
	}
	return keyList;
}

QString IngredientsParser::
AmountExtractor::findValueFor(const QString& word) const {
	map<QString, QString>::const_iterator it = m_elements.find(word);
	if(it != m_elements.end()) {
		return it->second;
	}
	return "";
}

void IngredientsParser::
AmountExtractor::compileNextNumber( QStringList &words, int sStart, int& end ) {
	findRangeInList(createKeyListFrom(m_elements), words, sStart, end);
	QString numberValue = findValueFor(buildStringFromRange(words, sStart, end));
	if(numberValue != "") {
		for(int i = sStart; i < end; ++i) {
			words[i] = "";
		}
		words[end - 1] = numberValue;
	}
}

IngredientsParser::str_list IngredientsParser::
ComponentExtracor::findItemsThatStartsWith(const str_list& list,  const QString& word) {
	str_list matchedProducts;
	str_list::const_iterator it = list.begin();
	for(; it != list.end(); ++it) {
		if(removePunctuationMarksFrom(*it).startsWith(removePunctuationMarksFrom(word))) {
			matchedProducts.push_back(*it);
		}
	}
	return matchedProducts;
}

QString IngredientsParser::
ComponentExtracor::buildStringFromRange(const QStringList& words, int start, int end) {
	QString result;
	for (int i = start; i < end; ++i) {
		result.append(words[i]).append(" ");
	}
	// result.chop(1); // remove last space
	return result.trimmed();
}

void IngredientsParser::
FromListExtractor::findMatchedRange(QStringList& words, int start, int& end) {
	findRangeInList(m_elements, words, start, end);
}

void IngredientsParser::
ComponentExtracor::findRangeInList(const std::vector<QString>& list, QStringList &words, int start, int& end ) {
	vector<QString> ing = findItemsThatStartsWith(list, words[start]);
	sort(ing.rbegin(), ing.rend(), stringLengthComprasion);
	str_list::const_iterator it = ing.begin();
	for(; it != ing.end(); ++it) { 
		int phraseEnd = start + it->split(" ").size();
		if(phraseEnd > words.size()) {
			continue;
		}
		if(twoItemsAreSemanticallyTheSame(*it, buildStringFromRange(words, start, phraseEnd))) {
			QStringList listTerm = it->split(" ");
			for(int i = 0; i < listTerm.size(); ++i) {
				words.replace(start + i, listTerm[i]);
			}
			end = phraseEnd;
			break;
		}
	}
}

bool IngredientsParser::
ComponentExtracor::twoItemsAreSemanticallyTheSame(const QString& item1, const QString& item2) {
	return removePunctuationMarksFrom(item1) == removePunctuationMarksFrom(item2);
}

QString IngredientsParser::
ComponentExtracor::removePunctuationMarksFrom(const QString& word) {
	QString w = word;
	w.replace(QString::fromStdWString(L"'"), QString::fromStdWString(L""));
	w.replace(QString::fromStdWString(L"\""), QString::fromStdWString(L""));
	w.replace(QString::fromStdWString(L","), QString::fromStdWString(L""));
	w.replace(QString::fromStdWString(L"("), QString::fromStdWString(L""));
	w.replace(QString::fromStdWString(L")"), QString::fromStdWString(L""));
	return w;

}

void IngredientsParser::
MethodExtractor::findMatchedRange(QStringList& words, int start, int& end) {
	end = words.size();
}

}


