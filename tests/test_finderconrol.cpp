#include <gmock/gmock.h>
#include "mock/mock_uifactory.h"
#include <core/db/recipedao.h>
#include <core/db/recipe.h>
#include "mock/mock_recipedao.h"
#include <core/eventmanager.h>
#include "mock/mock_recipeeditui.h"
#include <core/finder/findercontrol.h>
#include <core/recipesearchevent.h>
#include "mock/mock_ingredientdao.h"
#include <boost/assign.hpp>
#include <core/recipeedit/ingredientitem.h>
#include <core/recipeedit/recipeeditui.h>
#include "mock/mock_eventhandler.h"
#include <boost/pointer_cast.hpp>
#include <core/finder/recipefinder.h>

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
using Mamatkon::FinderControl;
using Mamatkon::RecipeSearchEvent;
using Mamatkon::IRecipeFinder;


class FinderControlTest : public ::testing::Test {
protected:
	EventManager eventManager;
	shared_ptr<MockRecipeDAO> recipeDao;
	shared_ptr<MockIngredientDao> ingredientDao;
	FinderControl* control;
	vector<long> foundRecipeIds;
	MockEventHandler* eventHandler;

	FinderControlTest() {
		control = new FinderControl(eventManager);
		recipeDao = shared_ptr<MockRecipeDAO>(new MockRecipeDAO);
		ingredientDao = shared_ptr<MockIngredientDao>(new MockIngredientDao);
		control->setRecipeDao(recipeDao);
		control->setIngredientDao(ingredientDao);
		foundRecipeIds = list_of (12) (3) (4);
		eventHandler = new MockEventHandler(eventManager, Event::RecipeSearch);
	}

	~FinderControlTest() {
		delete control;
		delete eventHandler;
	}
};

TEST_F(FinderControlTest, TestShouldDelegateRecipeSearchEventFilterByTitleAndInstructions) {
	QString str1 = "reduce";
	QString str2 = "heat";
	vector<QString> strs = list_of (str1) (str2);
	ON_CALL(*recipeDao, findRecipeTitleAndInstructions(strs)).WillByDefault(Return(foundRecipeIds));

	control->dispatchSearch(str1 + " " + str2, IRecipeFinder::TitleInstructions);
	EXPECT_TRUE(foundRecipeIds == static_pointer_cast<RecipeSearchEvent>(eventHandler->_Event)->recipeIds());
}

TEST_F(FinderControlTest, TestShouldDelegateRecipeSearchEventFilterByIngredients) {
	QString ing1("popo");
	QString ing2("meyoneze");
	vector<QString> ingredients = list_of (ing1) (ing2);
	ON_CALL(*ingredientDao, findRecipeIdsByIngredients(ingredients)).WillByDefault(Return(foundRecipeIds));

	control->dispatchSearch(ing1 + " , " + ing2 + ",", IRecipeFinder::Ingredients);
	EXPECT_TRUE(foundRecipeIds == static_pointer_cast<RecipeSearchEvent>(eventHandler->_Event)->recipeIds());
}

TEST_F(FinderControlTest, TestShouldDelegateRecipeSearchEventFilterByTags) {
	QString tag1("popo");
	QString tag2("meyoneze");
	vector<QString> tags = list_of (tag1) (tag2);
	ON_CALL(*recipeDao, findRecipeIdsByTags(tags)).WillByDefault(Return(foundRecipeIds));

	control->dispatchSearch(tag1 + " , " + tag2 + ",", IRecipeFinder::Tags);
	EXPECT_TRUE(foundRecipeIds == static_pointer_cast<RecipeSearchEvent>(eventHandler->_Event)->recipeIds());
}

TEST_F(FinderControlTest, TestShouldRaiseRecipeSearchClearEvent) {
	MockEventHandler eHandler = MockEventHandler(eventManager, Event::RecipeSearchClear);
	control->dispatchClearSearch();
	EXPECT_EQ(Event::RecipeSearchClear, eHandler._Event->type());
}

}
}