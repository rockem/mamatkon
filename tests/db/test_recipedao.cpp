#include <gtest/gtest.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <vector>
#include <core/db/qtrecipedao.h>
#include <core/db/qtdbutils.h>
#include "../stlutil.h"
#include <QStringList>
#include <boost/assign.hpp>

namespace MaTest {
namespace {

using ::std::vector;
using ::boost::shared_ptr;
using Mamatkon::QtDbUtils;
using Mamatkon::Recipe;
using Mamatkon::QtRecipeDao;
using boost::assign::list_of;

class QtRecipeDaoTest : public ::testing::Test {
protected:
    static const QString k_DbName;
	vector<Recipe> recipes;
    QSqlDatabase m_db;
	QtRecipeDao* m_dao;

    virtual void SetUp() {
        m_db = QSqlDatabase::addDatabase("QSQLITE", k_DbName);
        m_db.setDatabaseName(":memory:");
        m_db.open();
        QtDbUtils::createSchemaOn(m_db);
		m_dao = new QtRecipeDao(m_db);
		initDb();
    }

	void initDb() {
		recipes.push_back(Recipe(4, "bread", "do the and that", 2, 3, "mom", ""));
		recipes.push_back(Recipe(2, "choqlette", "nice and sweet", 3, 3, "", "winter"));

		createRecipeRow(recipes[0]);
		createRecipeRow(recipes[1]);
	}

    virtual void TearDown() {
        QSqlDatabase::database(k_DbName, false).close();
        QSqlDatabase::removeDatabase(k_DbName);
    }

    void createRecipeRow(const Recipe& recipe) {
        QSqlQuery q(m_db);
		q.prepare("insert into recipe "
			"(id, title, description, servings, servings_scale, source, tags) "
			"values(:id, :title, :desc, :servings, :servingsScale, :source, :tags)");
        q.bindValue(":id", QVariant::fromValue(recipe.Id));
        q.bindValue(":title", QVariant::fromValue(recipe.Title));
		q.bindValue(":desc", QVariant::fromValue(recipe.Instruction));
		q.bindValue(":servings", QVariant::fromValue(recipe.Servings));
		q.bindValue(":servingsScale", QVariant::fromValue(recipe.ServingsScale));
		q.bindValue(":source", QVariant::fromValue(recipe.Source));
		q.bindValue(":tags", QVariant::fromValue(recipe.Tags));

        q.exec();
        m_db.commit();
    }
};

const QString QtRecipeDaoTest::k_DbName = "in_mem_db";

TEST_F(QtRecipeDaoTest, TestShouldRetrieveMainRecipeData) {
    const vector<Recipe> list = m_dao->getAllRecipeTitles();
	EXPECT_TRUE(find(list.begin(), list.end(), recipes[0]) != list.end());
	EXPECT_TRUE(find(list.begin(), list.end(), recipes[1]) != list.end());
}

TEST_F(QtRecipeDaoTest, TestShouldRetrieveRecipeById) {
	Recipe r = m_dao->getRecipeById(recipes[0].Id);
	EXPECT_EQ(recipes[0], r);
}

TEST_F(QtRecipeDaoTest, TestShouldCreateNewRecipe) {
	long id = m_dao->createNewRecipe();

	Recipe r = m_dao->getRecipeById(id);
	EXPECT_EQ(Recipe(id, "No Title"), r);
}

TEST_F(QtRecipeDaoTest, TestShouldRetrievFirstRecipeId) {
	EXPECT_EQ(2, m_dao->getFirstRecipeId());
}

TEST_F(QtRecipeDaoTest, TestShouldUpdateRecipe) {
	Recipe recipe(recipes[0].Id, "popo");
	recipe.Instruction = "sdfgdfgbd";
	recipe.Servings = 4;
	recipe.ServingsScale = 4;
	recipe.Source = "mom";
	recipe.Tags = "winter";

	m_dao->updateRecipe(recipe);

	Recipe r2 = m_dao->getRecipeById(recipes[0].Id);
	EXPECT_EQ(recipe, r2);
}

TEST_F(QtRecipeDaoTest, TestShouldRetrieveRecipesTheContainsTheSearchInTitleOrDescription) {
	vector<QString> wordsToFind = list_of (recipes[0].Title) (recipes[1].Instruction.split(" ")[1]);
	vector<long> idsFound = m_dao->findRecipeTitleAndInstructions(wordsToFind);
	EXPECT_TRUE(find(idsFound.begin(), idsFound.end(), recipes[0].Id) != idsFound.end());
	EXPECT_TRUE(find(idsFound.begin(), idsFound.end(), recipes[1].Id) != idsFound.end());
}

TEST_F(QtRecipeDaoTest, TestShouldRetrieveRecipesTheContainsTheSearchInTags) {
	vector<QString> wordsToFind = list_of (recipes[1].Tags);
	vector<long> idsFound = m_dao->findRecipeIdsByTags(wordsToFind);
	EXPECT_TRUE(find(idsFound.begin(), idsFound.end(), recipes[1].Id) != idsFound.end());
}

}
}
