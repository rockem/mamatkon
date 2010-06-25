#include <gmock/gmock.h>
#include <core/recipegrabber/recipegrabbercontrol.h>
#include <core/eventmanager.h>
#include <QString>
#include <core/db/recipe.h>
#include <core/eventlistener.h>
#include "mock/mock_eventhandler.h"
#include <core/db/ingredient.h>
#include "mock/mock_ingredientdao.h"
#include <boost/assign.hpp>
#include <vector>
#include <core/recipeevent.h>


namespace MaTest {
namespace {

using ::boost::assign::list_of;
using ::boost::shared_ptr;
using ::std::vector;
using ::testing::Return;
using Mamatkon::Amount;
using Mamatkon::EventManager;
using Mamatkon::Event;
using Mamatkon::IRecipeGrabberEventListener;
using Mamatkon::Ingredient;
using Mamatkon::Measure;
using Mamatkon::Product;
using Mamatkon::Recipe;
using Mamatkon::RecipeGrabberControl;
using Mamatkon::RecipeEvent;
using ::boost::static_pointer_cast;

class RecipeGrabberControlTest_Ingredients : public ::testing::Test {
protected:
	EventManager eventManager;
	RecipeGrabberControl* control;
	shared_ptr<MockIngredientDao> ingredientDao;
	long currentRecipeId ;
	MockEventHandler* eventHandler;
	vector<Product> products;
	vector<Measure> measures;
	vector<Amount> amounts;
	vector<Ingredient> ingredients;


	RecipeGrabberControlTest_Ingredients() : currentRecipeId(3){
		control = new RecipeGrabberControl(eventManager);
		ingredientDao = shared_ptr<MockIngredientDao>(new MockIngredientDao);
		control->setIngredientDao(ingredientDao);

		eventManager.raiseEvent(
			shared_ptr<RecipeEvent>(new RecipeEvent(Event::SelectedRecipeChanged, currentRecipeId)));

		eventHandler = new MockEventHandler(eventManager, Event::RecipeUpdated);

		setupDefaultExpectations();
	}

	~RecipeGrabberControlTest_Ingredients() {
		EXPECT_TRUE(currentRecipeId == static_pointer_cast<RecipeEvent>(eventHandler->_Event)->recipeId());
		ingredients.clear();
		products.clear();
		measures.clear();
		amounts.clear();
		delete control;
		delete eventHandler;
	}

	QString convertLinesToSpaces(QString text) {
		return text.replace(QString::fromStdWString(L"_"), QString::fromStdWString(L" "));
	}

	QString createHebrewString(wchar_t* str) {
		return convertLinesToSpaces(QString::fromStdWString(str));
	}

	void setupExpectations()
	{
		EXPECT_CALL(*ingredientDao, addIngredientsToRecipe(currentRecipeId, ingredients));
		setupDefaultExpectations();
	}

	void setupDefaultExpectations() {
		ON_CALL(*ingredientDao, getProducts()).WillByDefault(Return(products));
		ON_CALL(*ingredientDao, getMeasures()).WillByDefault(Return(measures));
		ON_CALL(*ingredientDao, getAmounts()).WillByDefault(Return(amounts));
		ON_CALL(*ingredientDao, getMaxIdForRecipeId(currentRecipeId)).WillByDefault(Return(0));
	}

};

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldParseAmountProductInstructions) {
	products.push_back(Product(1, createHebrewString(L"תפוחי_אדמה")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "6", "", createHebrewString(L"בינוניים"), 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"6_תפוחי_אדמה_בינוניים"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldParseAmountMeasureProduct) {
	products.push_back(Product(1, createHebrewString(L"שמנת_מתוקה")));
	measures.push_back(Measure(1, createHebrewString(L"כוס")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "1", measures[0]._Name, "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"1_כוס_שמנת_מתוקה"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldParseAmountAsRange) {
	products.push_back(Product(1, createHebrewString(L"שמנת_מתוקה")));
	measures.push_back(Measure(1, createHebrewString(L"כוס")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "6-8", "", "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"6-8_שמנת_מתוקה"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, testShouldParseDifferentLinesAsDifferentIngrifdients) {
	products.push_back(Product(1, createHebrewString(L"לחם")));
	products.push_back(Product(2, createHebrewString(L"חלב")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "2", "", "", 0));
	ingredients.push_back(Ingredient(0, 2, products[1]._Name, "1", "", "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"2_לחם\n1_חלב"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldTreatQuotedMeasure) {
	products.push_back(Product(1, createHebrewString(L"אבקת_אפיה")));
	measures.push_back(Measure(1, createHebrewString(L"ג'")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "100", measures[0]._Name, "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"100_ג_אבקת_אפיה"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldTreatDoubleQoutedMeasure) {
	products.push_back(Product(1, createHebrewString(L"עגבניות")));
	measures.push_back(Measure(1, createHebrewString(L"ק\"ג")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "1", measures[0]._Name, "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"1_קג_עגבניות"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldParseAmountInWords) {
	products.push_back(Product(1, createHebrewString(L"שמן")));
	amounts.push_back(Amount(1, createHebrewString(L"מעט")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, amounts[0]._Name, "", "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"מעט_שמן"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldFindTheLongestProductName) {
	products.push_back(Product(1, createHebrewString(L"פלפל")));
	products.push_back(Product(2, createHebrewString(L"פלפל_שחור")));
	ingredients.push_back(Ingredient(0, 1, products[1]._Name, "", "", "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"פלפל_שחור"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldRemoveCommasFromProduct) {
	products.push_back(Product(1, createHebrewString(L"אורז_עגול")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "", "", "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"אורז_עגול,"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldNotCreateEntryOnEmptyLine) {
	EXPECT_CALL(*ingredientDao, addIngredientsToRecipe(currentRecipeId, ingredients)).Times(0);
	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L""));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldParseAlternateIngredient) {
	products.push_back(Product(1, createHebrewString(L"כוסברה")));
	products.push_back(Product(2, createHebrewString(L"פטרוזיליה")));
	measures.push_back(Measure(1, createHebrewString(L"חבילות")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "2", measures[0]._Name, "", 0));
	ingredients.push_back(Ingredient(0, 2, products[1]._Name, "2", measures[0]._Name, "", 1));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"2_חבילות_כוסברה_(או_פטרוזיליה("));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldParseAlternateIngredientWithNothingBeforeTheOR) {
	products.push_back(Product(1, createHebrewString(L"סוכר")));
	products.push_back(Product(2, createHebrewString(L"חמאה")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "2", "", "", 0));
	ingredients.push_back(Ingredient(0, 2, products[1]._Name, "2", "", "", 1));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"2_סוכר_או_חמאה"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldNotMatchOtherProductsWithTheSameLength) {
	products.push_back(Product(1, createHebrewString(L"כוסברה_ירוקה")));
	products.push_back(Product(2, createHebrewString(L"כוסברה_שחורה")));
	ingredients.push_back(Ingredient(0, 1, products[1]._Name, "", "", "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"כוסברה_שחורה"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldParseMoreThanOneAlternateProducts) {
	products.push_back(Product(1, createHebrewString(L"חלב")));
	products.push_back(Product(2, createHebrewString(L"חובצה")));
	products.push_back(Product(3, createHebrewString(L"לבן")));
	products.push_back(Product(4, createHebrewString(L"יוגורט")));
	measures.push_back(Measure(1, createHebrewString(L"כוסות")));
	// ***
	// FIXME: =========== this "(" should be fixed !!! ===============
	// ***
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "1", measures[0]._Name, "(", 0));
	ingredients.push_back(Ingredient(0, 2, products[1]._Name, "1", measures[0]._Name, "", 1));
	ingredients.push_back(Ingredient(0, 3, products[2]._Name, "1", measures[0]._Name, "", 1));
	ingredients.push_back(Ingredient(0, 4, products[3]._Name, "1", measures[0]._Name, "", 1));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"1_כוסות_חלב_(_או_חובצה,_או_לבן,_או_יוגורט("));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldParseWholeAndFractionAmount) {
	products.push_back(Product(1, createHebrewString(L"חלב")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "1 3/4", "", "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"3/4_1_חלב"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldParseAlternateSeperatedWithBackslash) {
	products.push_back(Product(1, createHebrewString(L"אורז")));
	products.push_back(Product(2, createHebrewString(L"ירקות")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "2", "", "", 0));
	ingredients.push_back(Ingredient(0, 2, products[1]._Name, "2", "", "", 1));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"2_אורז_/_2_ירקות"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldParseAmountRangeInWords) {
	products.push_back(Product(1, createHebrewString(L"עגבניות")));
	amounts.push_back(Amount(1, createHebrewString(L"אחד"), "1"));
	amounts.push_back(Amount(2, createHebrewString(L"חצי"), "1/2"));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "1 - 1 1/2", "", "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"אחד_עד_אחד_וחצי_עגבניות"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, testShouldParseAmountAndFractionInWords) {
	products.push_back(Product(1, createHebrewString(L"מלפפונים")));
	amounts.push_back(Amount(1, createHebrewString(L"אחד"), "1"));
	amounts.push_back(Amount(2, createHebrewString(L"חצי"), "1/2"));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "1 1/2", "", "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"אחד_וחצי_מלפפונים"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldParseAmountRangeWithSpaces) {
	products.push_back(Product(1, createHebrewString(L"מלפפונים")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, createHebrewString(L"2_-_3"), "", "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"2_-_3_מלפפונים"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldParseAlternateAmountsSeparatedByOr) {
	products.push_back(Product(1, createHebrewString(L"חמאה")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "1", "", createHebrewString(L"במחבת_/_במיקרו"), 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"1_חמאה_במחבת_או_במיקרו"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldNumberInWordAtTheEnd) {
	products.push_back(Product(1, createHebrewString(L"לימון")));
	amounts.push_back(Amount(1, createHebrewString(L"אחד"), "1"));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "1", "", "", 0));
	setupExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, createHebrewString(L"לימון_אחד"));
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldUpdateIdsToAdjustExistingIngredients) {
	QString row = createHebrewString(L"1_כוס_שמנת_מתוקה");
	products.push_back(Product(1, createHebrewString(L"שמנת_מתוקה")));
	measures.push_back(Measure(1, createHebrewString(L"כוס")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "1", measures[0]._Name, "", 0));
	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, row);

	ingredients[0]._Id = 2;
	setupExpectations();
	ON_CALL(*ingredientDao, getMaxIdForRecipeId(currentRecipeId)).WillByDefault(Return(1));
	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, row);
}

TEST_F(RecipeGrabberControlTest_Ingredients, TestShouldUpdateIdsToAdjustExistingAlternateIngredients) {
	QString row = createHebrewString(L"2_סוכר_או_חמאה");
	products.push_back(Product(1, createHebrewString(L"סוכר")));
	products.push_back(Product(2, createHebrewString(L"חמאה")));
	ingredients.push_back(Ingredient(0, 1, products[0]._Name, "2", "", "", 0));
	ingredients.push_back(Ingredient(0, 2, products[1]._Name, "2", "", "", 1));
	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, row);

	ingredients[0]._Id = 3;
	ingredients[1]._Id = 4;
	ingredients[1]._AltId = 3;
	setupExpectations();
	ON_CALL(*ingredientDao, getMaxIdForRecipeId(currentRecipeId)).WillByDefault(Return(2));
	control->dispatchGrab(IRecipeGrabberEventListener::Ingredients, row);
}

}
}
