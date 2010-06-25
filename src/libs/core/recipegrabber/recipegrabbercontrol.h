#ifndef RECIPEGRABBERCONTROL_H
#define RECIPEGRABBERCONTROL_H
#include <core/eventlistener.h>
#include <core/controlbase.h>
#include <boost/shared_ptr.hpp>
#include <vector>

class QString;

namespace Mamatkon {

class EventManager;
class IRecipeDAO;
class IIngredientDao;
class EventArgs;
struct Recipe;
class Event;
class IngredientsParser;

class RecipeGrabberControl : public ControlBase, public IRecipeGrabberEventListener {

	long m_currentRecipeId;

	void onSelectedRecipeChanged(::boost::shared_ptr<Event> e);
	void updateRecipe(IRecipeGrabberEventListener::RecipeSection section, const QString& text);
	void updateIngredients(const QString& text);
	void updateIngredientsIds(::std::vector<Ingredient> &ing);
	IngredientsParser createParser();
public:
	RecipeGrabberControl(EventManager& em);

	void dispatchGrab(IRecipeGrabberEventListener::RecipeSection section, const QString& text);
};





}
#endif // RECIPEGRABBERCONTROL_H
