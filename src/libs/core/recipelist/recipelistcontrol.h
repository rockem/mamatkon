#ifndef __RECIPELIST_H
#define __RECIPELIST_H
#include <boost/shared_ptr.hpp>
#include <core/db/recipedao.h>
#include <core/controlbase.h>

namespace Mamatkon {

	struct IUiFactory;
	struct IRecipeListUi;
	struct Recipe;
	class EventManager;
	class Event;

class RecipeListControl : public ControlBase, public ISelectionEventListener {

	long m_selectedRecipeId;

	void onRecipeListChanged(::boost::shared_ptr<Event> e);
	void onSelectedRecipeChanged(::boost::shared_ptr<Event> e);
	void onRecipeSearch(::boost::shared_ptr<Event> e);
public:
	RecipeListControl(EventManager& em);

	void dispatchSelectionChanged(long recipeId);
};

}
#endif
