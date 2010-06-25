#include "recipegrabbercontrol.h"
#include <core/eventmanager.h>
#include <core/db/recipedao.h>
#include <core/db/ingredientdao.h>
#include <boost/assign.hpp>
#include <QString>
#include <core/recipegrabber/ingredientsparser.h>
#include <core/db/ingredient.h>
#include <core/recipeevent.h>
#include <boost/pointer_cast.hpp>
#include <map>

namespace Mamatkon {

using ::std::map;
using ::boost::shared_ptr;
using ::std::vector;
using ::boost::static_pointer_cast;

RecipeGrabberControl::RecipeGrabberControl(EventManager& em) : ControlBase(em) {
	eventManager().addEventListener(Event::SelectedRecipeChanged, METHOD_MEMEBER(RecipeGrabberControl::onSelectedRecipeChanged));
}

void RecipeGrabberControl::dispatchGrab(IRecipeGrabberEventListener::RecipeSection section, const QString& text) {
	if(section == IRecipeGrabberEventListener::Ingredients) {
		updateIngredients(text);
	} else {
		updateRecipe(section, text);
	}
	eventManager().raiseEvent(shared_ptr<RecipeEvent>(new RecipeEvent(Event::RecipeUpdated, m_currentRecipeId)));
}

void RecipeGrabberControl::onSelectedRecipeChanged(shared_ptr<Event> e) {
	m_currentRecipeId = static_pointer_cast<RecipeEvent>(e)->recipeId();
}

void RecipeGrabberControl::updateRecipe(IRecipeGrabberEventListener::RecipeSection section, const QString& text ) {
	Recipe recipe = recipeDao()->getRecipeById(m_currentRecipeId);
	switch(section) {
		case IRecipeGrabberEventListener::Title:
			recipe.Title = text;
			break;
		case IRecipeGrabberEventListener::Instructions:
			recipe.Instruction = text;
			break;
		case IRecipeGrabberEventListener::Source:
			recipe.Source = text;
			break;
		case IRecipeGrabberEventListener::Servings:
			recipe.Servings = text.toInt();
			break;
	}
	recipeDao()->updateRecipe(recipe);
}

template<typename T>
std::vector<QString> createStringListFrom(std::vector<T> data) {
	vector<QString> stringList;
	typename vector<T>::const_iterator it = data.begin();
	for(; it != data.end(); ++it) { 
		stringList.push_back(it->_Name);
	}
	return stringList;
}

template<typename T>
map<QString, QString> createStringMapFrom(vector<T> data) {
	map<QString, QString> stringMap;
	typename vector<T>::const_iterator it = data.begin();
	for(; it != data.end(); ++it) { 
		stringMap[it->_Name] = it->_Value;
	}
	return stringMap;
}

void RecipeGrabberControl::updateIngredients(const QString& text) {
	vector<Ingredient> ings = createParser().parseIngridients(text);
	updateIngredientsIds(ings);
	if(ings.size() > 0) {
		ingredientDao()->addIngredientsToRecipe(m_currentRecipeId, ings);
	}
}

IngredientsParser RecipeGrabberControl::createParser() {
	IngredientsParser parser;
	parser.setAmounts(createStringMapFrom(ingredientDao()->getAmounts()));
	parser.setProducts(createStringListFrom(ingredientDao()->getProducts()));
	parser.setMeasures(createStringListFrom(ingredientDao()->getMeasures()));
	return parser;
}

void RecipeGrabberControl::updateIngredientsIds( vector<Ingredient> &ing ) {
	int maxId = ingredientDao()->getMaxIdForRecipeId(m_currentRecipeId);
	vector<Ingredient>::iterator it = ing.begin();
	for (; it != ing.end(); ++it) {
		it->_Id += maxId;
		if(it->_AltId > 0) {
			it->_AltId += maxId;
		}
	}
}

}
