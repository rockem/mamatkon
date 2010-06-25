#include <gtest/gtest.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <vector>
#include <core/db/qtingredientdao.h>
#include <core/db/qtdbutils.h>
#include "../stlutil.h"
#include <core/db/ingredient.h>
#include <boost/assign.hpp>
#include <core/db/nutritionaldata.h>

namespace MaTest {
namespace {

using ::std::vector;
using ::boost::shared_ptr;
using Mamatkon::QtDbUtils;
using Mamatkon:: QtIngredientDao;
using Mamatkon::Amount;
using Mamatkon::Product;
using Mamatkon::Measure;
using Mamatkon::Ingredient;
using boost::assign::list_of;
using Mamatkon::NutritionalData;
using boost::assign::map_list_of;

class QtIngredientDaoTest : public ::testing::Test {
protected:
	static const QString k_DbName;
	QSqlDatabase m_db;
	QtIngredientDao* m_dao;

	virtual void SetUp() {
		m_db = QSqlDatabase::addDatabase("QSQLITE", k_DbName);
		m_db.setDatabaseName(":memory:");
		m_db.open();
		QtDbUtils::createSchemaOn(m_db);
		m_dao = new QtIngredientDao(m_db);
	}

	virtual void TearDown() {
		QSqlDatabase::database(k_DbName, false).close();
		QSqlDatabase::removeDatabase(k_DbName);
	}

	void insertRow(const QString& table, const QString& name) {
		QSqlQuery q(m_db);
		q.prepare("insert into " + table + " (name) values(:name)");
		q.bindValue(":name", name);
		q.exec();
	}

	void insertRow(const Measure& measure) {
		QSqlQuery q(m_db);
		q.prepare("insert into measure (name, ratioToGram) values(:name, :ratioToGram)");
		q.bindValue(":name", measure._Name);
		q.bindValue(":ratioToGram", measure._RatioToGram);
		q.exec();
	}

	void insertAmount(const Amount& amount) {
		QSqlQuery q(m_db);
		q.prepare("insert into amount (name, value) values(:name, :value)");
		q.bindValue(":name", amount._Name);
		q.bindValue(":value", amount._Value);
		q.exec();
	}

	void insertIngredient(const Ingredient& ing) {
		QSqlQuery q(m_db);
		q.prepare("insert into ingredient " 
			"(recipe_id, ingredient_id, product, amount, measure, method, alt_id) "
			"values(:recipeId, :ingredientId, :product, :amount, :measure, :method, :alt_id)");
        q.bindValue(":recipeId", QVariant::fromValue(ing._RecipeId));
        q.bindValue(":ingredientId", QVariant::fromValue(ing._Id));
        q.bindValue(":product", QVariant::fromValue(ing._ProductId));
        q.bindValue(":amount", QVariant::fromValue(ing._Amount));
        q.bindValue(":measure", QVariant::fromValue(ing._MeasureId));
        q.bindValue(":method", QVariant::fromValue(ing._Method));
        q.bindValue(":alt_id", QVariant::fromValue(ing._AltId));
		q.exec();
	}
};

const QString QtIngredientDaoTest::k_DbName = "in_mem_db";

TEST_F(QtIngredientDaoTest, TestShouldRetrieveAllAmounts) {
	Amount amount1(0, "am1", "1");
	Amount amount2(0, "am2", "1/3");
	insertAmount(amount1);
	insertAmount(amount2);

	vector<Amount> amounts = m_dao->getAmounts();
	EXPECT_TRUE(amount1 == amounts[0]);
	EXPECT_TRUE(amount2 == amounts[1]);
}

TEST_F(QtIngredientDaoTest, TestShouldRetrieveAllProducts) {
	QString product1 = "gr";
	QString product2 = "kilo";
	insertRow("product", product1);
	insertRow("product", product2);

	vector<Product> products = m_dao->getProducts();
	EXPECT_TRUE(product1 == products[0]._Name);
	EXPECT_TRUE(product2 == products[1]._Name);
}

TEST_F(QtIngredientDaoTest, TestShouldRetrieveAllMeasues) {
	Measure measure1(0, "gr", 1);
	Measure measure2(0, "kilo", 1000);
	insertRow(measure1);
	insertRow(measure2);

	vector<Measure> measures = m_dao->getMeasures();
	EXPECT_TRUE(measure1 == measures[0]);
	EXPECT_TRUE(measure2 == measures[1]);
}

TEST_F(QtIngredientDaoTest, TestShouldInsertNewIngredient) {
	vector<Ingredient> ingredients;
	ingredients.push_back(Ingredient(0, 1, "lala", "2", "", "" , 0));

	long recipeId = 2;
	m_dao->addIngredientsToRecipe(recipeId, ingredients);

	EXPECT_TRUE(ingredients == m_dao->getIngredientsForRecipeId(recipeId));
}

TEST_F(QtIngredientDaoTest, TestShouldInsertMoreThanOneRecipeIngredient) {
	vector<Ingredient> ingredients = list_of
		(Ingredient(0, 1, "lala", "2", "", "" , 0))
		(Ingredient(0, 2, "popo", "3/4", "", "" , 0));

	long recipeId = 2;
	m_dao->addIngredientsToRecipe(recipeId, ingredients);

	EXPECT_TRUE(ingredients == m_dao->getIngredientsForRecipeId(recipeId));
}

TEST_F(QtIngredientDaoTest, TestShouldRetrieveRecipeIdsWithIngredientsSpecified) {
	vector<Ingredient> ingredients = list_of
		(Ingredient(54, 1, "lala", "2", "", "" , 0))
	    (Ingredient(23, 2, "popo", "3/4", "", "" , 0));
	insertIngredient(ingredients[0]);
	insertIngredient(ingredients[1]);

	vector<QString> ings = list_of (ingredients[1]._ProductId) ("non exist");
	vector<long> ids = m_dao->findRecipeIdsByIngredients(ings);

	EXPECT_EQ(1, ids.size());
	EXPECT_EQ(ingredients[1]._RecipeId, ids[0]);
}

TEST_F(QtIngredientDaoTest, TestShouldDeleteIngredient) {
	Ingredient ing(54, 1, "lala", "2", "", "" , 0);
	insertIngredient(ing);

	m_dao->deleteIngredient(ing._RecipeId, ing._Id);

	EXPECT_EQ(0, m_dao->getIngredientsForRecipeId(ing._RecipeId).size());
}

TEST_F(QtIngredientDaoTest, TestShouldRetrieveIngredientMaxId) {
	long recipeId = 12;
	EXPECT_EQ(0, m_dao->getMaxIdForRecipeId(recipeId));
	Ingredient ing(recipeId, 1, "lala", "2", "", "" , 0);
	insertIngredient(ing);

	int i = m_dao->getMaxIdForRecipeId(recipeId);
	EXPECT_EQ(1, i);
}

TEST_F(QtIngredientDaoTest, TestShouldDeleteAlternateIngredients) {
	long recipeId = 54;
	vector<Ingredient> ingredients = list_of
		(Ingredient(recipeId, 1, "lala", "2", "", "" , 0))
		(Ingredient(recipeId, 2, "popo", "3/4", "", "" , 1));
	insertIngredient(ingredients[0]);
	insertIngredient(ingredients[1]);

	m_dao->deleteIngredient(recipeId, ingredients[0]._Id);

	EXPECT_EQ(0, m_dao->getIngredientsForRecipeId(recipeId).size());
}

TEST_F(QtIngredientDaoTest, TestShouldUpdateIngredient) {
	long recipeId = 34;
	Ingredient ing(recipeId, 1, "lala", "2", "gr", "" , 0);
	insertIngredient(ing);

	ing._ProductId = "pop";
	m_dao->updateIngredient(recipeId, ing);
	vector<Ingredient> i = m_dao->getIngredientsForRecipeId(recipeId);
	EXPECT_TRUE(ing == i[0]);
}

TEST_F(QtIngredientDaoTest, TestShouldRetrieveNutritionalDataForProduct) {
	QString product1 = "Honey";
	QString product2 = "Bread";
	NutritionalData n1;
	n1._UnitWeight = 100;
	n1._NutritionalValues[NutritionalData::B12] = (float)0.1;
	n1._NutritionalValues[NutritionalData::Calories] = (float)112;
	NutritionalData n2;
	n2._UnitWeight = 100;
	n2._NutritionalValues[NutritionalData::B12] = (float)11;
	n2._NutritionalValues[NutritionalData::Calories] = (float)115;
	m_dao->insertNutritionalData(product1, n1);
	m_dao->insertNutritionalData(product2, n2);

	NutritionalData nd(m_dao->getNutritionalDataFor(product1));
	EXPECT_TRUE(n1 == m_dao->getNutritionalDataFor(product1));
	EXPECT_TRUE(n2 == m_dao->getNutritionalDataFor(product2));
}

TEST_F(QtIngredientDaoTest, TestShouldRetrieveRatioToGramForMeasure) {
	Measure measure1(0, "gr", 1);
	Measure measure2(0, "kilo", 1000);
	insertRow(measure1);
	insertRow(measure2);

	EXPECT_EQ(measure2._RatioToGram, m_dao->getRatioToGramFor(measure2._Name));
}

}
}
