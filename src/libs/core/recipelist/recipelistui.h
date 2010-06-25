#ifndef _RECIPELISTUI_H
#define _RECIPELISTUI_H
#include <vector>
#include <boost/shared_ptr.hpp>
#include <core/ui/uibase.h>

namespace Mamatkon {
    struct Recipe;
}

namespace Mamatkon {

typedef ::boost::shared_ptr<Recipe> RecipePtr;

struct IRecipeListUi : public UiBase {
public:
    virtual ~IRecipeListUi() {}
    virtual void updateRecipeList(const std::vector<Recipe>& recipes) = 0;
	virtual long getSelectedRecipeId() const = 0;
	virtual void setSelectedRecipeTo(long recipeId) = 0;
};

}
#endif
