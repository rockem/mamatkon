#ifndef RECIPEUPDATER_H
#define RECIPEUPDATER_H
#include "eventlistener.h"

namespace Mamatkon { 

class IngredientItem;

class IRecipeUpdater : public IEventListener {
public:
	virtual ~IRecipeUpdater() {}
	virtual void dispatchRecipeUpdate(const Recipe& recipe) = 0;
	virtual void dispatchDeleteIngredient(long id) = 0;
	virtual void dispatchIngredientsUpdate(IngredientItem* item) = 0;
};

}
#endif // RECIPEUPDATER_H