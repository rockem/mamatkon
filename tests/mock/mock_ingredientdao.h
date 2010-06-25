#ifndef MOCK_INGREDIENTDAO_H
#define MOCK_INGREDIENTDAO_H
#include <core/db/ingredientdao.h>
#include <core/db/ingredient.h>
#include <core/db/nutritionaldata.h>

namespace MaTest {

class MockIngredientDao : public Mamatkon::IIngredientDao {
public:
	MOCK_METHOD2(addIngredientsToRecipe, void(long recipeId, ::std::vector<Mamatkon::Ingredient> ingredients));
	MOCK_CONST_METHOD1(getIngredientsForRecipeId, ::std::vector<Mamatkon::Ingredient>(long recipeId));
	MOCK_CONST_METHOD0(getProducts, ::std::vector<Mamatkon::Product>());
	MOCK_CONST_METHOD0(getMeasures, ::std::vector<Mamatkon::Measure>());
	MOCK_CONST_METHOD0(getAmounts, ::std::vector<Mamatkon::Amount>());
	MOCK_CONST_METHOD1(findRecipeIdsByIngredients, ::std::vector<long>(::std::vector<QString> ingredients));
	MOCK_METHOD2(deleteIngredient, void(long recipeId, int ingredientId));
	MOCK_CONST_METHOD1(getMaxIdForRecipeId, int(long recipeId));
	MOCK_METHOD2(updateIngredient, void(long recipeId, const Mamatkon::Ingredient& ingredient));
	MOCK_CONST_METHOD1(getNutritionalDataFor, Mamatkon::NutritionalData(const QString& iName));
	MOCK_METHOD2(insertNutritionalData, void(const QString& product, const Mamatkon::NutritionalData& data));
	MOCK_CONST_METHOD1(getRatioToGramFor, float(const QString& measure));
};



}

#endif // MOCK_INGREDIENTDAO_HW