#ifndef INGREDIENT_H
#define INGREDIENT_H
#include <QString>

namespace Mamatkon {

struct Product {
	long _Id;
	QString _Name;

	Product(long id, QString name) : _Id(id), _Name(name) {}
};

struct Amount {
	long _Id;
	QString _Name;
	QString _Value;

	Amount(long id, QString name, QString size = "") : _Id(id), _Name(name), _Value(size) {}

	bool operator==(const Amount& other) const {
		return 
			_Name == other._Name &&
			_Value == other._Value;
	}

	bool operator!=(const Amount& other) const {
		return !(*this == other);
	}
};

struct Measure {
	long _Id;
	QString _Name;
	float _RatioToGram;

	Measure(long id, QString name, float ratio = 1) : _Id(id), _Name(name), _RatioToGram(ratio) {}

	bool operator==(const Measure& other) const {
		return 
			_Name == other._Name &&
			_RatioToGram == other._RatioToGram;
	}

	bool operator!=(const Measure& other) const {
		return !(*this == other);
	}
};

struct Ingredient {
	long _RecipeId;
	int _Id;
	QString _ProductId;
	QString _Amount;
	QString _MeasureId;
	QString _Method;
	int _AltId;

	Ingredient() : _RecipeId(0), _AltId(0) {}


	Ingredient(
		long recipeId,
		int id, 
		QString productId,
		QString amount,
		QString measureId,
		QString method,
		int altId) : 
	_RecipeId(recipeId),
		_Id(id),
		_ProductId(productId),
		_Amount(amount),
		_MeasureId(measureId),
		_Method(method),
		_AltId(altId) {}

	bool operator==(const Ingredient& other) const {
		return 
			_Id == other._Id &&
			_ProductId == other._ProductId &&
			_Amount == other._Amount &&
			_MeasureId == other._MeasureId &&
			_Method == other._Method &&
			_AltId == other._AltId;
	}

	bool operator!=(const Ingredient& other) const {
		return !(*this == other);
	}
};

}
#endif // INGREDIENT_H
