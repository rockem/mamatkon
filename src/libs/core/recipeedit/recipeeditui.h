#ifndef RECIPEEDITUI_H
#define RECIPEEDITUI_H
#include <core/ui/uibase.h>
#include <boost/shared_ptr.hpp>

namespace Mamatkon { 

struct Recipe;
class IngredientItem;

class IRecipeEditUi : public UiBase{
public:
	virtual ~IRecipeEditUi() {}
	virtual void updateRecipeFields(Recipe recipe) = 0;
	virtual void updateIngredients(::boost::shared_ptr<IngredientItem> ingredients) = 0;

};


}
#endif // RECIPEEDITUI_H