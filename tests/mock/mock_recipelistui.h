#ifndef MOCK_RECIPELISTUI_H
#define MOCK_RECIPELISTUI_H
#include <core/recipelist/recipelistui.h>

namespace MaTest {

class MockRecipeListUi : public Mamatkon::IRecipeListUi {
public:
	MOCK_METHOD1(updateRecipeList, void(const ::std::vector<Mamatkon::Recipe> & recipes));
	MOCK_CONST_METHOD0(getSelectedRecipeId, long());
	MOCK_METHOD1(setSelectedRecipeTo, void(long));
};

}

#endif // MOCK_RECIPELISTUI_H
