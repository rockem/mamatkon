#include <gmock/gmock.h>
#include "mock/mock_nutritionaldataui.h"
#include "mock/mock_ingredientdao.h"
#include "mock/mock_recipedao.h"
#include <core/nuritionaldata/nutritionaldatacontrol.h>
#include <core/recipeevent.h>
#include "mock/mock_nutritionaldataui.h"
#include <boost/assign.hpp>
#include <core/fraction.h>

namespace MaTest {
namespace {

	using namespace Mamatkon;
	using ::boost::shared_ptr;
	using ::testing::Return;
	using ::std::vector;
	using ::std::map;
	using ::boost::assign::map_list_of;
	
	class NutritionalDataControlTest : public ::testing::Test {
	protected:
		EventManager eventManager;
		NutritionalDataControl* control;
		shared_ptr<MockRecipeDAO> recipeDao;
		shared_ptr<MockIngredientDao> ingredientDao;
		long recipeId;
		vector<Ingredient> ingredients;
		MockNutritionalDataUi stubUi;
		
		NutritionalDataControlTest() {
			control = new NutritionalDataControl(eventManager);
			control->setUi(&stubUi);
			control->setRecipeDao(recipeDao = shared_ptr<MockRecipeDAO>(new MockRecipeDAO));
			control->setIngredientDao(ingredientDao = shared_ptr<MockIngredientDao>(new MockIngredientDao));

			recipeId = 34;
		}

		~NutritionalDataControlTest() {
			delete control;
		}

		void fireRecipeChangedAndExpectations() {
			ON_CALL(*ingredientDao, getIngredientsForRecipeId(recipeId)).WillByDefault(Return(ingredients));
			ON_CALL(*ingredientDao, getRatioToGramFor(QString("gr"))).WillByDefault(Return(1));
			eventManager.raiseEvent(shared_ptr<RecipeEvent>(new RecipeEvent(Event::SelectedRecipeChanged, recipeId)));
		}

	};

TEST_F(NutritionalDataControlTest, TestShouldDelegateNutritionalValueToView) {
	ingredients.push_back(Ingredient(0, 1, "Sugar", "50", "gr", "", 0));
	ingredients.push_back(Ingredient(0, 2, "Honey", "75", "gr", "", 0));

	NutritionalData n1(100, map_list_of(NutritionalData::Calories, (float)398) (NutritionalData::Carbohydrate, (float)99.4));
	NutritionalData n2(100, map_list_of(NutritionalData::Calories, (float)350) (NutritionalData::Carbohydrate, (float)50) (NutritionalData::Fat, (float)5));

	ON_CALL(*ingredientDao, getNutritionalDataFor(ingredients[0]._ProductId)).WillByDefault(Return(n1));
	ON_CALL(*ingredientDao, getNutritionalDataFor(ingredients[1]._ProductId)).WillByDefault(Return(n2));

	map<NutritionalData::Types, float> nd;
	nd[NutritionalData::Calories] = 50 / (float)100 * 398 + 75 / (float)100 * 350;
	nd[NutritionalData::Carbohydrate] = 50 / (float)100 * (float)99.4 + 75 / (float)100 * 50;
	nd[NutritionalData::Fat] = 75 / (float)100 * 5;

	EXPECT_CALL(stubUi, setData(nd));

	fireRecipeChangedAndExpectations();
}

TEST_F(NutritionalDataControlTest, TestShouldHandleAmountWithFractions) {
	ingredients.push_back(Ingredient(0, 1, "Sugar", "12 1/2", "gr", "", 0));
	ingredients.push_back(Ingredient(0, 2, "Honey", "3/4", "gr", "", 0));

	NutritionalData n1(100, map_list_of(NutritionalData::Calories, (float)398) (NutritionalData::Carbohydrate, (float)99.4));
	NutritionalData n2(100, map_list_of(NutritionalData::Calories, (float)350) (NutritionalData::Carbohydrate, (float)50) (NutritionalData::Fat, (float)5));

	ON_CALL(*ingredientDao, getNutritionalDataFor(ingredients[0]._ProductId)).WillByDefault(Return(n1));
	ON_CALL(*ingredientDao, getNutritionalDataFor(ingredients[1]._ProductId)).WillByDefault(Return(n2));

	float amount1 = Fraction(ingredients[0]._Amount).toFloat();
	float amount2 = Fraction(ingredients[1]._Amount).toFloat();
	map<NutritionalData::Types, float> nd;
	nd[NutritionalData::Calories] = amount1 / 100 * 398 + amount2 / 100 * 350;
	nd[NutritionalData::Carbohydrate] = amount1 / 100 * (float)99.4 + amount2 / (float)100 * 50;
	nd[NutritionalData::Fat] = amount2 / 100 * 5;

	EXPECT_CALL(stubUi, setData(nd));

	fireRecipeChangedAndExpectations();
}

TEST_F(NutritionalDataControlTest, TestShouldCalculateNutritionValuesWithDifferentMeasures) {
	ingredients.push_back(Ingredient(0, 1, "Sugar", "2 1/2", "spoon", "", 0));
	ingredients.push_back(Ingredient(0, 2, "Honey", "3/4", "cup", "", 0));

	NutritionalData n1(100, map_list_of(NutritionalData::Calories, (float)398) (NutritionalData::Carbohydrate, (float)99.4));
	NutritionalData n2(100, map_list_of(NutritionalData::Calories, (float)350) (NutritionalData::Carbohydrate, (float)50) (NutritionalData::Fat, (float)5));

	ON_CALL(*ingredientDao, getNutritionalDataFor(ingredients[0]._ProductId)).WillByDefault(Return(n1));
	ON_CALL(*ingredientDao, getNutritionalDataFor(ingredients[1]._ProductId)).WillByDefault(Return(n2));

	ON_CALL(*ingredientDao, getRatioToGramFor(QString("spoon"))).WillByDefault(Return(15));
	ON_CALL(*ingredientDao, getRatioToGramFor(QString("cup"))).WillByDefault(Return(160));

	float amount1 = Fraction(ingredients[0]._Amount).toFloat() * 15;
	float amount2 = Fraction(ingredients[1]._Amount).toFloat() * 160;
	map<NutritionalData::Types, float> nd;
	nd[NutritionalData::Calories] = amount1 / 100 * 398 + amount2 / 100 * 350;
	nd[NutritionalData::Carbohydrate] = amount1 / 100 * (float)99.4 + amount2 / (float)100 * 50;
	nd[NutritionalData::Fat] = amount2 / 100 * 5;

	EXPECT_CALL(stubUi, setData(nd));

	fireRecipeChangedAndExpectations();
}






}
}
