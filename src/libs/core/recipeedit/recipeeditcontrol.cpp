#include "recipeeditcontrol.h"
#include "ingredientitem.h"
#include <core/recipeevent.h>
#include <boost/pointer_cast.hpp>
#include <core/recipeedit/recipeeditui.h>
#include <core/db/ingredient.h>
#include <boost/math/common_factor.hpp>
#include <QRegExp>
#include <QStringList>
#include "numberscaler.h"
#include <iostream>

using namespace Mamatkon;

using ::boost::shared_ptr;
using ::boost::static_pointer_cast;
using ::std::vector;
using ::boost::math::gcd;

RecipeEditControl::RecipeEditControl( EventManager& em ) : 
ControlBase(em), 
	m_currentRecipeId(0), 
	m_scaleRatio("0") {
	eventManager().addEventListener(Event::SelectedRecipeChanged, METHOD_MEMEBER(RecipeEditControl::onRecipeSelectionChanged));
}

void RecipeEditControl::onRecipeSelectionChanged(::boost::shared_ptr<Event> e) {
	m_currentRecipeId = static_pointer_cast<RecipeEvent>(e)->recipeId();
	updateUiRecipeData();
	updateUiIngredientsData();
}

void RecipeEditControl::updateUiRecipeData() {
	Recipe recipe = recipeDao()->getRecipeById(m_currentRecipeId);
	m_scaleRatio = QString::number(recipe.ServingsScale) + "/" + QString::number(recipe.Servings);
	static_cast<IRecipeEditUi*>(getUi())->updateRecipeFields(recipe);
}

void RecipeEditControl::updateUiIngredientsData() {
	vector<Ingredient> ingredients = ingredientDao()->getIngredientsForRecipeId(m_currentRecipeId);
	shared_ptr<IngredientItem> rootItem(new IngredientItem(createIngredientsHeader()));

	addIngridientsToTree(ingredients, rootItem);
	addAlternateIngrridientsToTree(ingredients, rootItem);

	static_cast<IRecipeEditUi*>(getUi())->updateIngredients(rootItem);
}

void RecipeEditControl::addIngridientsToTree(vector<Ingredient> &ingredients, shared_ptr<IngredientItem> &rootItem) {
	vector<Ingredient>::const_iterator it = ingredients.begin();
	for (; it != ingredients.end(); ++it) {
		if(it->_AltId == 0) {
			addIngrerdientRowTo(it, rootItem.get());
		}
	}
}

void RecipeEditControl::addIngrerdientRowTo(vector<Ingredient>::const_iterator it, IngredientItem* item) {
	QList<QVariant> row;
	row << it->_Amount << it->_MeasureId << it->_ProductId << it->_Method << scaleAmount(it->_Amount);
	item->appendChild(new IngredientItem(row, it->_Id, item));
}

QString RecipeEditControl::scaleAmount(const QString& amount) {
	QStringList amounts = amount.split("-");
	QString newAmount = NumberScaler(m_scaleRatio).getScaledNumber(amounts[0].trimmed());
	if(amounts.size() > 1) {
		newAmount += "-" + NumberScaler(m_scaleRatio).getScaledNumber(amounts[1].trimmed());
	}
	return newAmount;
}

void RecipeEditControl::addAlternateIngrridientsToTree(
	vector<Ingredient> &ingredients, shared_ptr<IngredientItem> rootItem) {
	vector<Ingredient>::const_iterator it = ingredients.begin();
	for (; it != ingredients.end(); ++it) {
		if(it->_AltId > 0) {
			for (int i = 0; i < rootItem->childCount(); ++i) {
				if(rootItem->child(i)->id() == it->_AltId) {
					addIngrerdientRowTo(it, rootItem->child(i));
				}
			}
		}
	}
}

QList<QVariant> RecipeEditControl::createIngredientsHeader() {
	static const char* columns[] = {
		QT_TR_NOOP("Amount"),
		QT_TR_NOOP("Measure"),
		QT_TR_NOOP("Product"),
		QT_TR_NOOP("Method"),
		QT_TR_NOOP("Scaled"),
	};

	QList<QVariant> header;
	header << columns[0] << columns[1] << columns[2] << columns[3] << columns[4];
	return header;
}

void RecipeEditControl::dispatchRecipeUpdate(const Recipe& recipe) {
	Recipe r = recipe;
	r.Id = m_currentRecipeId;
	recipeDao()->updateRecipe(r);
	eventManager().raiseEvent(shared_ptr<RecipeEvent>(new RecipeEvent(Event::RecipeUpdated, m_currentRecipeId)));
}

void Mamatkon::RecipeEditControl::dispatchDeleteIngredient(long id) {
	ingredientDao()->deleteIngredient(m_currentRecipeId, id);
	eventManager().raiseEvent(shared_ptr<RecipeEvent>(new RecipeEvent(Event::RecipeUpdated, m_currentRecipeId)));
}

void Mamatkon::RecipeEditControl::dispatchIngredientsUpdate(IngredientItem* item) {
	Ingredient ing(0, 
		item->id(), 
		item->data(2).toString(), 
		item->data(0).toString(), 
		item->data(1).toString(), 
		item->data(3).toString(), 
		0);
	if(item->parent() != 0) {
		ing._AltId = item->parent()->id();
	}
	ingredientDao()->updateIngredient(m_currentRecipeId, ing);
}
