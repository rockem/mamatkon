#include "nutritionaldatacontrol.h"
#include <core/db/ingredient.h>
#include <core/db/nutritionaldata.h>
#include <core/recipeevent.h>
#include <boost/pointer_cast.hpp>
#include <core/nuritionaldata/nutritionaldataui.h>
#include <core/fraction.h>

using namespace Mamatkon;
using ::std::vector;
using ::std::map;
using ::boost::static_pointer_cast;

Mamatkon::NutritionalDataControl::NutritionalDataControl( EventManager& em ) : 
ControlBase(em) {
	eventManager().addEventListener(
		Event::SelectedRecipeChanged, METHOD_MEMEBER(NutritionalDataControl::onRecipeSelectionChanged));
}

void NutritionalDataControl::onRecipeSelectionChanged(::boost::shared_ptr<Event> e) {
	map<NutritionalData::Types, float> nData;
	vector<Ingredient> ingredients = 
		ingredientDao()->getIngredientsForRecipeId(static_pointer_cast<RecipeEvent>(e)->recipeId());

	vector<Ingredient>::const_iterator it = ingredients.begin();
	for (; it != ingredients.end(); ++it) {
		NutritionalData nd = ingredientDao()->getNutritionalDataFor(it->_ProductId);
		float ratioToGram = ingredientDao()->getRatioToGramFor(it->_MeasureId);
		map<NutritionalData::Types, float>::const_iterator ndIt = nd._NutritionalValues.begin();
		if(nd._UnitWeight != 0) { ////// ============== hack !!!!!
			for (; ndIt != nd._NutritionalValues.end(); ++ndIt) {
				nData[ndIt->first] += Fraction(it->_Amount).toFloat() * ratioToGram / nd._UnitWeight * ndIt->second;
			}
		}
	}

	dynamic_cast<INutritionalDataUi*>(getUi())->setData(nData);
}