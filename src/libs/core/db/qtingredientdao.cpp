#include "qtingredientdao.h"
#include <core/db/ingredient.h>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <iostream>
#include <set>

using namespace Mamatkon;

using ::std::vector;
using ::std::set;
using ::std::map;

template< class T>
class AllFetcher {

	QSqlDatabase& m_db;
protected:
	AllFetcher(QSqlDatabase& db) : m_db(db) {}
	virtual T createStructFrom(const QSqlQuery& q) const = 0;

public:
	vector<T> fetchFromTable(const QString& tableName) {
		QSqlQuery q(m_db);
		q.exec(QString("select * from " +  tableName));
		vector<T> data;
		while(q.next()) {
			data.push_back(createStructFrom(q));
		}
		return data;
	}
};

class AmountAllFetcher : public AllFetcher<Amount> {
public:
    AmountAllFetcher(QSqlDatabase& db) : AllFetcher<Amount>(db) {}
protected:
	Amount createStructFrom(const QSqlQuery& q) const {
		return Amount(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString());
	}
};

class MeasureAllFetcher : public AllFetcher<Measure> {
public:
	MeasureAllFetcher(QSqlDatabase& db) : AllFetcher<Measure>(db) {}
protected:
	Measure createStructFrom(const QSqlQuery& q) const {
		return Measure(q.value(0).toInt(), q.value(1).toString(), q.value(2).toDouble());
	}
};

class ProductAllFetcher : public AllFetcher<Product> {
public:
	ProductAllFetcher(QSqlDatabase& db) : AllFetcher<Product>(db) {}
protected:
	Product createStructFrom(const QSqlQuery& q) const {
		return Product(q.value(0).toInt(), q.value(1).toString());
	}
};

void QtIngredientDao::addIngredientsToRecipe(long recipeId, ::std::vector<Ingredient> ingredients) {
	QSqlQuery q(database());
	q.prepare("insert into ingredient " 
		"(recipe_id, ingredient_id, product, amount, measure, method, alt_id) "
		"values(:recipeId, :ingredientId, :product, :amount, :measure, :method, :alt_id)");
	vector<Ingredient>::const_iterator it = ingredients.begin();
	for (; it != ingredients.end(); ++it) {
		q.bindValue(":recipeId", QVariant::fromValue(recipeId));
		q.bindValue(":ingredientId", it->_Id);
		q.bindValue(":product", it->_ProductId);
		q.bindValue(":amount", it->_Amount);
		q.bindValue(":measure", it->_MeasureId);
		q.bindValue(":method", it->_Method);
		q.bindValue(":alt_id", it->_AltId);
		q.exec();
	}
}

::std::vector<Product> QtIngredientDao::getProducts() const {
	return ProductAllFetcher(database()).fetchFromTable("product");
}

::std::vector<Measure> QtIngredientDao::getMeasures() const {
	return MeasureAllFetcher(database()).fetchFromTable("measure");
}

::std::vector<Amount> QtIngredientDao::getAmounts() const {
	return AmountAllFetcher(database()).fetchFromTable("amount");
}

::std::vector<Ingredient> QtIngredientDao::getIngredientsForRecipeId( long recipeId ) const {
	QSqlQuery q(database());
	q.prepare("select * from ingredient where recipe_id = :recipeId order by ingredient_id");
	q.bindValue(":recipeId", QVariant::fromValue(recipeId));
	q.exec();
	vector<Ingredient> ingredients;
	while(q.next()) {
		ingredients.push_back(
			Ingredient(
			q.value(1).toLongLong(),
			q.value(2).toInt(),
			q.value(3).toString(),
			q.value(4).toString(),
			q.value(5).toString(),
			q.value(6).toString(),
			q.value(7).toInt()));
	}
	return ingredients;
}

::std::vector<long> QtIngredientDao::findRecipeIdsByIngredients(::std::vector<QString> ingredients) const {
	return findRecipeIdsBy("select recipe_id from ingredient where product like '%%1%'", ingredients);
}

void QtIngredientDao::deleteIngredient( long recipeId, int ingredientId ) {
	QSqlQuery q(database());
	q.exec(QString("delete from ingredient where recipe_id = %1 and (ingredient_id = %2 or alt_id = %2)").
		arg(QString::number(recipeId), QString::number(ingredientId)));
}

int QtIngredientDao::getMaxIdForRecipeId(long recipeId) const {
	QSqlQuery q(database());
	q.exec(QString("select max(ingredient_id) from ingredient where recipe_id = %1").
		arg(QString::number(recipeId)));
	q.next();
	return q.value(0).toInt();
}

void Mamatkon::QtIngredientDao::updateIngredient(long recipeId, const Ingredient& ingredient) {
	QSqlQuery q(database());
	q.exec(
		QString("update ingredient set amount='%1', measure='%2', product='%3', method='%4' where recipe_id = %5 and ingredient_id = %6").
		arg(
		ingredient._Amount, 
		ingredient._MeasureId, 
		ingredient._ProductId, 
		ingredient._Method, 
		QString::number(recipeId), 
		QString::number(ingredient._Id)));
}

float Mamatkon::QtIngredientDao::getRatioToGramFor( const QString& measure ) const {
	QSqlQuery q(database());
	q.exec(QString("select ratioToGram from measure where name = '%1'").arg(measure));
	if(q.next()) {
		return q.value(0).toDouble();
	}
	return 0;
}

NutritionalData QtIngredientDao::getNutritionalDataFor(const QString& product) const {
	QSqlQuery q(database());
	q.exec(QString("select * from nutritional_data where product = '%1'").arg(product));
	NutritionalData data;
	if(q.next()) {
		data._UnitWeight = q.value(2).toInt();
		data._NutritionalValues[NutritionalData::Calories    ] = q.value(3).toDouble();
		data._NutritionalValues[NutritionalData::Protein     ] = q.value(4).toDouble();
		data._NutritionalValues[NutritionalData::Carbohydrate] = q.value(5).toDouble();
		data._NutritionalValues[NutritionalData::Fat         ] = q.value(6).toDouble();
		data._NutritionalValues[NutritionalData::Iron        ] = q.value(7).toDouble();
		data._NutritionalValues[NutritionalData::Calcium     ] = q.value(8).toDouble();
		data._NutritionalValues[NutritionalData::VitaminC    ] = q.value(9).toDouble();
		data._NutritionalValues[NutritionalData::Potassium   ] = q.value(10).toDouble();
		data._NutritionalValues[NutritionalData::Fiber       ] = q.value(11).toDouble();
		data._NutritionalValues[NutritionalData::VitaminA    ] = q.value(12).toDouble();
		data._NutritionalValues[NutritionalData::Cholesterol ] = q.value(13).toDouble();
		data._NutritionalValues[NutritionalData::SaturatedFat] = q.value(14).toDouble();
		data._NutritionalValues[NutritionalData::B12         ] = q.value(15).toDouble();
		data._NutritionalValues[NutritionalData::Sodium      ] = q.value(16).toDouble();
		data._NutritionalValues[NutritionalData::PholicAcid  ] = q.value(17).toDouble();
		data._NutritionalValues[NutritionalData::Omega3      ] = q.value(18).toDouble();
	}
	return data;
}

QString QtIngredientDao::getValueFor(const ::std::map<NutritionalData::Types, float>& data, NutritionalData::Types type ) const {
	map<NutritionalData::Types, float>::const_iterator it = data.find(type);
	if(it != data.end()) {
		return QString::number(it->second);
	}
	return QString::number(0);
}

void QtIngredientDao::insertNutritionalData( const QString& product, const NutritionalData& data ) {
	QSqlQuery q(database());
	q.prepare("insert into nutritional_data ("
		"product, "
		"unit_weight, " 
		"calories, "
		"protein, "
		"carbohydrate, " 
		"fat, "
		"iron, "
		"calcium, " 
		"vitaminc, "
		"potassium, "
		"fiber, "
		"vitamina, " 
		"cholesterol, " 
		"saturated_fat, "
		"b12, "
		"sodium, " 
		"pholic_acid, " 
		"omega3) "
		"values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

	q.addBindValue(QVariant::fromValue(product));
	q.addBindValue(QVariant::fromValue(data._UnitWeight));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::Calories)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::Protein)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::Carbohydrate)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::Fat)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::Iron)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::Calcium)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::VitaminC)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::Potassium)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::Fiber)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::VitaminA)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::Cholesterol)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::SaturatedFat)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::B12)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::Sodium)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::PholicAcid)));
	q.addBindValue(QVariant::fromValue(getValueFor(data._NutritionalValues, NutritionalData::Omega3)));

	q.exec();
}
