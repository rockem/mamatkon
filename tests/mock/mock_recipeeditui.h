#ifndef MOCK_RECIPEEDITUI_H
#define MOCK_RECIPEEDITUI_H
#include <core/recipeedit/recipeeditui.h>

namespace MaTest { 

class MockRecipeEditUi : public Mamatkon::IRecipeEditUi {
public:
	MOCK_METHOD1(updateRecipeFields, void(Mamatkon::Recipe recipe));
	MOCK_METHOD1(updateIngredients, void(::boost::shared_ptr<Mamatkon::IngredientItem> ingredients));
};

}
#endif // MOCK_RECIPEEDITUI_H