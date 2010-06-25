#include "recipelistcontrol.h"
#include <core/ui/uifactory.h>
#include <core/recipelist/recipelistui.h>
#include <core/db/recipe.h>
#include <core/eventmanager.h>
#include <boost/pointer_cast.hpp>
#include <core/recipeevent.h>
#include <vector>
#include <core/recipesearchevent.h>

namespace Mamatkon {

using ::boost::static_pointer_cast;
using ::boost::shared_ptr;
using ::std::vector;

RecipeListControl::RecipeListControl(EventManager& em) : 
ControlBase(em), m_selectedRecipeId(0) {
	eventManager().addEventListener(Event::RecipeListChanged, METHOD_MEMEBER(RecipeListControl::onRecipeListChanged));
	eventManager().addEventListener(Event::SelectedRecipeChanged, METHOD_MEMEBER(RecipeListControl::onSelectedRecipeChanged));
	eventManager().addEventListener(Event::RecipeUpdated, METHOD_MEMEBER(RecipeListControl::onRecipeListChanged));
	eventManager().addEventListener(Event::RecipeSearch, METHOD_MEMEBER(RecipeListControl::onRecipeSearch));
	eventManager().addEventListener(Event::RecipeSearchClear, METHOD_MEMEBER(RecipeListControl::onRecipeListChanged));
}

void RecipeListControl::dispatchSelectionChanged(long recipeId) {
	eventManager().raiseEvent(shared_ptr<RecipeEvent>(new RecipeEvent(Event::SelectedRecipeChanged, recipeId)));
}

void RecipeListControl::onRecipeListChanged(shared_ptr<Event> e) {
	// long id = dynamic_cast<IRecipeListUi*>(getUi())->getSelectedRecipeId();
	dynamic_cast<IRecipeListUi*>(getUi())->updateRecipeList(recipeDao()->getAllRecipeTitles());
	dynamic_cast<IRecipeListUi*>(getUi())->setSelectedRecipeTo(m_selectedRecipeId);
}

void RecipeListControl::onSelectedRecipeChanged(shared_ptr<Event> e) {
	m_selectedRecipeId = static_pointer_cast<RecipeEvent>(e)->recipeId();
	dynamic_cast<IRecipeListUi*>(
		getUi())->setSelectedRecipeTo(m_selectedRecipeId);
}

void RecipeListControl::onRecipeSearch(::boost::shared_ptr<Event> e) {
	vector<long> ids = static_pointer_cast<RecipeSearchEvent>(e)->recipeIds();
	vector<Recipe> recipes;
	vector<long>::const_iterator it = ids.begin();
	for(; it != ids.end(); ++it) {
		recipes.push_back(recipeDao()->getRecipeById(*it));
	}
	dynamic_cast<IRecipeListUi*>(getUi())->updateRecipeList(recipes);
}
	

}
