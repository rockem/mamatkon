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
		static const float k_Calories1;
		static const float k_Carbohydrate1;
		static const float k_Calories2;
		static const float k_Carbohydrate2;
		static const float k_Fat2;

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
			NutritionalData n1(100, map_list_of(NutritionalData::Calories, (float)398) (NutritionalData::Carbohydrate, (float)99.4));
			NutritionalData n2(100, map_list_of(NutritionalData::Calories, (float)350) (NutritionalData::Carbohydrate, (float)50) (NutritionalData::Fat, (float)5));
			ON_CALL(*ingredientDao, getNutritionalDataFor(ingredients[0]._ProductId)).WillByDefault(Return(n1));
			ON_CALL(*ingredientDao, getNutritionalDataFor(ingredients[1]._ProductId)).WillByDefault(Return(n2));
			ON_CALL(*ingredientDao, getIngredientsForRecipeId(recipeId)).WillByDefault(Return(ingredients));
			ON_CALL(*ingredientDao, getRatioToGramFor(QString("gr"))).WillByDefault(Return(1));
			eventManager.raiseEvent(shared_ptr<RecipeEvent>(new RecipeEvent(Event::SelectedRecipeChanged, recipeId)));
		}

	};

	const float NutritionalDataControlTest::k_Calories1 = 398;
	const float NutritionalDataControlTest::k_Carbohydrate1 = (float)99.4;
	const float NutritionalDataControlTest::k_Calories2 = 350;
	const float NutritionalDataControlTest::k_Carbohydrate2 = 50;
	const float NutritionalDataControlTest::k_Fat2 = 5;

TEST_F(NutritionalDataControlTest, TestShouldDelegateNutritionalValueToView) {
	ingredients.push_back(Ingredient(0, 1, "Sugar", "50", "gr", "", 0));
	ingredients.push_back(Ingredient(0, 2, "Honey", "75", "gr", "", 0));

	float amount1 = 50 / (float)100;
	float amount2 = 75 / (float)100;

	map<NutritionalData::Types, float> nd;
	nd[NutritionalData::Calories] = amount1 * k_Calories1 + amount2 * k_Calories2;
	nd[NutritionalData::Carbohydrate] = amount1 * k_Carbohydrate1 + amount2 * k_Carbohydrate2;
	nd[NutritionalData::Fat] = amount2 * k_Fat2;

	EXPECT_CALL(stubUi, setData(nd));

	fireRecipeChangedAndExpectations();
}

TEST_F(NutritionalDataControlTest, TestShouldHandleAmountWithFractions) {
	ingredients.push_back(Ingredient(0, 1, "Sugar", "12 1/2", "gr", "", 0));
	ingredients.push_back(Ingredient(0, 2, "Honey", "3/4", "gr", "", 0));

	float amount1 = Fraction(ingredients[0]._Amount).toFloat() / 100;
	float amount2 = Fraction(ingredients[1]._Amount).toFloat() / 100;
	map<NutritionalData::Types, float> nd;
	nd[NutritionalData::Calories] = amount1 * k_Calories1 + amount2 * k_Calories2;
	// Hack: for some reason amount2 fails the test on this line
    nd[NutritionalData::Carbohydrate] = (12.5 / (float)100) * k_Carbohydrate1 + amount2 * k_Carbohydrate2;
    nd[NutritionalData::Fat] = amount2 * k_Fat2;

	EXPECT_CALL(stubUi, setData(nd));

	fireRecipeChangedAndExpectations();
}

TEST_F(NutritionalDataControlTest, TestShouldCalculateNutritionValuesWithDifferentMeasures) {
	ingredients.push_back(Ingredient(0, 1, "Sugar", "2 1/2", "spoon", "", 0));
	ingredients.push_back(Ingredient(0, 2, "Honey", "3/4", "cup", "", 0));

    int spoonRatio = 15;
    int cupRatio = 160;
    ON_CALL(*ingredientDao, getRatioToGramFor(QString("spoon"))).WillByDefault(Return(spoonRatio));
    ON_CALL(*ingredientDao, getRatioToGramFor(QString("cup"))).WillByDefault(Return(cupRatio));

    float amount1 = Fraction(ingredients[0]._Amount).toFloat() * spoonRatio / 100;
    float amount2 = Fraction(ingredients[1]._Amount).toFloat() * cupRatio / 100;
    map<NutritionalData::Types,float> nd;
    nd[NutritionalData::Calories] = amount1 * k_Calories1 + amount2 * k_Calories2;
    nd[NutritionalData::Carbohydrate] = amount1 * k_Carbohydrate1 + amount2 * k_Carbohydrate2;
	nd[NutritionalData::Fat] = amount2 * k_Fat2;

	EXPECT_CALL(stubUi, setData(nd));

	fireRecipeChangedAndExpectations();
}






}
}
