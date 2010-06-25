#include "qtrecipedao.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QApplication>
#include <set>

namespace Mamatkon {

using ::std::vector;
using ::std::set;

const vector<Recipe> QtRecipeDao::getAllRecipeTitles() const {
    QSqlQuery q(database());
    q.exec("select * from recipe order by title");
    vector<Recipe> recipes;
    while(q.next()) {
        recipes.push_back(createRecipeFrom(q));
    }
    return recipes;
}

Recipe QtRecipeDao::createRecipeFrom(QSqlQuery& query) const {
    Recipe recipe(query.value(0).toLongLong(), query.value(1).toString());
	recipe.Instruction = query.value(2).toString();
	recipe.Servings = query.value(3).toInt();
	recipe.ServingsScale = query.value(4).toInt();
	recipe.Source = query.value(5).toString();
	recipe.Tags = query.value(6).toString();
    return recipe;
}

Recipe QtRecipeDao::getRecipeById(long id) const {
	QSqlQuery q(database());
	q.prepare("select * from recipe where id = :id");
	q.bindValue(":id", QVariant::fromValue(id));
	q.exec();
	q.next();
	return createRecipeFrom(q);
}

long QtRecipeDao::createNewRecipe() {
	QSqlQuery q(database());
	q.prepare("insert into recipe "
		"(title, description, servings, servings_scale, source) "
		"values(:title, :desc, :servings, :servingsScale, :source)" );

	Recipe recipe(0, qApp->translate("QtRecipeDao", "No Title"));
	q.bindValue(":title", QVariant::fromValue(recipe.Title));
	q.bindValue(":desc", QVariant::fromValue(recipe.Instruction));
	q.bindValue(":servings", QVariant::fromValue(recipe.Servings));
	q.bindValue(":servingsScale", QVariant::fromValue(recipe.ServingsScale));
	q.bindValue(":source", QVariant::fromValue(recipe.Source));
	q.exec();
	q.prepare("select last_insert_rowid()");
	q.exec();
	q.next();
	return q.value(0).toLongLong();
}

long QtRecipeDao::getFirstRecipeId() const {
	QSqlQuery q(database());
	q.exec("select id from recipe order by id limit 1");
	q.next();
	return q.value(0).toLongLong();
}

void QtRecipeDao::updateRecipe(const Recipe& recipe) {
	QSqlQuery q(database());
	q.prepare("update recipe set "
		"title=:title, "
		"description=:desc, "
		"servings=:servings, "
		"servings_scale=:servingsScale, "
		"tags=:tags, "
		"source=:source where id=:id");
	q.bindValue(":title", QVariant::fromValue(recipe.Title));
	q.bindValue(":desc", QVariant::fromValue(recipe.Instruction));
	q.bindValue(":servings", QVariant::fromValue(recipe.Servings));
	q.bindValue(":servingsScale", QVariant::fromValue(recipe.ServingsScale));
	q.bindValue(":tags", QVariant::fromValue(recipe.Tags));
	q.bindValue(":source", QVariant::fromValue(recipe.Source));
	q.bindValue(":id", QVariant::fromValue(recipe.Id));
	q.exec();
}

std::vector<long> QtRecipeDao::findRecipeTitleAndInstructions( std::vector<QString> words ) const {
	return findRecipeIdsBy("select id from recipe where title like '%%1%' or description like '%%1%'", words);	
}

std::vector<long> QtRecipeDao::findRecipeIdsByTags( std::vector<QString> words) const {
	return findRecipeIdsBy("select id from recipe where tags like '%%1%'", words);
}
}
