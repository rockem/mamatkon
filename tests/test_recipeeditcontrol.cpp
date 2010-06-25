#include <gmock/gmock.h>
#include "mock/mock_uifactory.h"
#include <core/db/recipedao.h>
#include <core/db/recipe.h>
#include "mock/mock_recipedao.h"
#include <core/eventmanager.h>
#include "mock/mock_recipeeditui.h"
#include <core/recipeedit/recipeeditcontrol.h>
#include <core/recipeevent.h>
#include "mock/mock_ingredientdao.h"
#include <boost/assign.hpp>
#include <core/recipeedit/ingredientitem.h>
#include <core/recipeedit/recipeeditui.h>
#include "mock/mock_eventhandler.h"
#include <boost/pointer_cast.hpp>


namespace MaTest {
namespace {

using ::boost::assign::list_of;
using ::boost::shared_ptr;
using ::boost::static_pointer_cast;
using ::std::vector;
using ::testing::Return;
using Mamatkon::Event;
using Mamatkon::EventManager;
using Mamatkon::IRecipeEditUi;
using Mamatkon::Ingredient;
using Mamatkon::IngredientItem;
using Mamatkon::Recipe;
using Mamatkon::RecipeEditControl;
using Mamatkon::RecipeEvent;


class RecipeEditControlTest : public ::testing::Test {
protected:
	class StubRecipeEditUi : public IRecipeEditUi {
	public:
		shared_ptr<IngredientItem> updatedIngredients;
		Recipe updatedRecipe; 

		void updateRecipeFields(Recipe recipe) {
			updatedRecipe = recipe;
		}

		void updateIngredients(shared_ptr<IngredientItem> ingredients) {
			updatedIngredients = ingredients;
		}
	};

	EventManager eventManager;
	RecipeEditControl* control;
	shared_ptr<MockRecipeDAO> recipeDao;
	shared_ptr<MockIngredientDao> ingredientDao;
	StubRecipeEditUi stubUi;
	long recipeId;
	vector<Ingredient> ingredients;
	Recipe* recipe;

	RecipeEditControlTest() : recipeId(11) {
		recipeDao = shared_ptr<MockRecipeDAO>(new MockRecipeDAO);
		ingredientDao = shared_ptr<MockIngredientDao>(new MockIngredientDao);
		control = new RecipeEditControl(eventManager);
		control->setRecipeDao(recipeDao);
		control->setIngredientDao(ingredientDao);
		control->setUi(&stubUi);
		recipe = 0;
	
	}

	void defaultExpectations() {
		if(recipe == 0) {
			recipe = new Recipe();
		}
		ON_CALL(*ingredientDao, getIngredientsForRecipeId(recipeId)).WillByDefault(Return(ingredients));
		ON_CALL(*recipeDao, getRecipeById(recipeId)).WillByDefault(Return(*recipe));
	}

	~RecipeEditControlTest() {
		delete control;
		delete recipe;
	}

	void compareRowTo( IngredientItem* row, Ingredient ingredient )  {
		EXPECT_EQ(row->id(), ingredient._Id);
		EXPECT_TRUE(row->data(0) == ingredient._Amount);
		EXPECT_TRUE(row->data(1) == ingredient._MeasureId);
		EXPECT_TRUE(row->data(2) == ingredient._ProductId);
		EXPECT_TRUE(row->data(3) == ingredient._Method);
	}

	QList<QString> createRowFrom( const Ingredient& ingredient )  {
		QList<QString> row;
		row << ingredient._Amount <<
			ingredient._MeasureId <<
			ingredient._ProductId <<
			ingredient._Method;
		return row;
	}

	void fireRecipeSelectedChangedEvent() {
		defaultExpectations();
		eventManager.raiseEvent(shared_ptr<RecipeEvent>(new RecipeEvent(Event::SelectedRecipeChanged, recipeId)));
	}
};


TEST_F(RecipeEditControlTest, TestShouldDelegateRecipeDataToViewOnSelectedRecipeChangedEvent) {
	recipe = new Recipe(recipeId, "popo");

	fireRecipeSelectedChangedEvent();

	EXPECT_EQ(stubUi.updatedRecipe, *recipe);
}

TEST_F(RecipeEditControlTest, TestShouldDelegateIngredientTreeToView) {
	ingredients.push_back(Ingredient(recipeId, 1, "bread", "3", "gr", "", 0));
	ingredients.push_back(Ingredient(recipeId, 2, "butter", "12", "units", "as is", 0));

	fireRecipeSelectedChangedEvent();

	compareRowTo(stubUi.updatedIngredients->child(0), ingredients[0]);
	compareRowTo(stubUi.updatedIngredients->child(1), ingredients[1]);
}

TEST_F(RecipeEditControlTest, TestShouldInsertAltenateIngridientsAsChildElements) {
	ingredients = list_of 
		(Ingredient(recipeId, 1, "bread", "3", "gr", "", 0)) 
		(Ingredient(recipeId, 2, "butter", "12", "units", "as is", 1));

	fireRecipeSelectedChangedEvent();

	IngredientItem* firstRow = stubUi.updatedIngredients->child(0);
	compareRowTo(firstRow->child(0), ingredients[1]);
	EXPECT_EQ(firstRow, firstRow->child(0)->parent());
}

TEST_F(RecipeEditControlTest, TestShouldUpdateRecipeOnDispatchRecipeUpdtae) {
	fireRecipeSelectedChangedEvent();

	MockEventHandler eventHandler(eventManager, Event::RecipeUpdated);
	Recipe recipe(recipeId, "lala");
	EXPECT_CALL(*recipeDao, updateRecipe(recipe));

	recipe.Id = 0;
	control->dispatchRecipeUpdate(recipe);

	EXPECT_EQ(recipeId, static_pointer_cast<RecipeEvent>(eventHandler._Event)->recipeId());
}

TEST_F(RecipeEditControlTest, TestShouldDelegateScaledAmountsOnScaleOfOne) {
	recipe = new Recipe(recipeId, "popo");
	recipe->ServingsScale = 1;

	ingredients = list_of 
		(Ingredient(recipeId, 1, "bread", "2/3", "gr", "", 0)) 
		(Ingredient(recipeId, 2, "butter", "12", "units", "as is", 0));

	fireRecipeSelectedChangedEvent();

	EXPECT_TRUE(QString("2/3") == stubUi.updatedIngredients->child(0)->data(4));
	EXPECT_TRUE(QString("12") == stubUi.updatedIngredients->child(1)->data(4));
}

TEST_F(RecipeEditControlTest, TestShouldDelegateScaledAmounts) {
	recipe = new Recipe(recipeId, "popo");
	recipe->Servings = 2;
	recipe->ServingsScale = 5;

	ingredients = list_of 
		(Ingredient(recipeId, 1, "bread", "3 1/2", "gr", "", 0)) 
		(Ingredient(recipeId, 2, "butter", "12", "units", "as is", 0))
		(Ingredient(recipeId, 3, "lala", "3/4", "units", "as is", 0))
		(Ingredient(recipeId, 3, "aaa", "3", "gr", "as is", 0));

	fireRecipeSelectedChangedEvent();

	EXPECT_TRUE(QString("8 3/4") == stubUi.updatedIngredients->child(0)->data(4));
	EXPECT_TRUE(QString("30") == stubUi.updatedIngredients->child(1)->data(4));
	EXPECT_TRUE(QString("1 7/8") == stubUi.updatedIngredients->child(2)->data(4));
	EXPECT_TRUE(QString("7 1/2") == stubUi.updatedIngredients->child(3)->data(4));
}

TEST_F(RecipeEditControlTest, TestShouldDelegateScaledRangedAmounts) {
	recipe = new Recipe(recipeId, "popo");
	recipe->Servings = 2;
	recipe->ServingsScale = 5;

	ingredients = list_of 
		(Ingredient(recipeId, 1, "bread", "3 1/2-2", "gr", "", 0))
		(Ingredient(recipeId, 2, "bread", "3 1/2 - 1", "gr", "", 0));

	fireRecipeSelectedChangedEvent();

	EXPECT_TRUE(QString("8 3/4-5") == stubUi.updatedIngredients->child(0)->data(4));
	EXPECT_TRUE(QString("8 3/4-2 1/2") == stubUi.updatedIngredients->child(1)->data(4));
}

TEST_F(RecipeEditControlTest, TestShouldDeleteIngredient) {
	fireRecipeSelectedChangedEvent();

	MockEventHandler eventHandler(eventManager, Event::RecipeUpdated);
	int ingredientId = 4;
	EXPECT_CALL(*ingredientDao, deleteIngredient(recipeId, ingredientId));

	control->dispatchDeleteIngredient(ingredientId);

	EXPECT_EQ(recipeId, static_pointer_cast<RecipeEvent>(eventHandler._Event)->recipeId());
}

TEST_F(RecipeEditControlTest, TestShouldDelegateIngredientsToDaoOnDispatchUpdate) {
	fireRecipeSelectedChangedEvent();

	Ingredient ingredient(0, 3, "2/3", "gr", "butter", "", 0);
	EXPECT_CALL(*ingredientDao, updateIngredient(recipeId, ingredient));

	QList<QVariant> l;
	l << ingredient._Amount << ingredient._MeasureId << ingredient._ProductId << ingredient._Method;
	IngredientItem* item = new IngredientItem(l, ingredient._Id);
	control->dispatchIngredientsUpdate(item);

	delete item;
}

}
}