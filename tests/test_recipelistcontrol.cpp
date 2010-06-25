#include <gmock/gmock.h>
#include "mock/mock_uifactory.h"
#include "stlutil.h"
#include <QString>
#include <core/recipelist/recipelistcontrol.h>
#include <core/db/recipedao.h>
#include <core/db/recipe.h>
#include <core/recipelist/recipelistui.h>
#include <core/ui/uifactory.h>
#include <vector>
#include "mock/mock_recipedao.h"
#include "mock/mock_recipelistui.h"
#include "mock/mock_eventhandler.h"
#include <core/eventmanager.h>
#include <core/recipeevent.h>
#include <boost/pointer_cast.hpp>
#include <boost/assign.hpp>
#include <core/recipesearchevent.h>

namespace MaTest {
namespace {

using ::boost::shared_ptr;
using ::std::vector;
using ::testing::Return;
using Mamatkon::CommandCommander;
using Mamatkon::IRecipeListUi;
using Mamatkon::ISelectionEventListener;
using Mamatkon::Recipe;
using Mamatkon::RecipeListControl;
using Mamatkon::EventManager;
using Mamatkon::Event;
using Mamatkon::RecipeEvent;
using ::boost::static_pointer_cast;
using ::boost::assign::list_of;
using Mamatkon::RecipeSearchEvent;


class StubRecipeListUi : public IRecipeListUi {
public:
	vector<Recipe> m_listRecieved;
	int m_selectedId;

	void updateRecipeList(const vector<Recipe>& recipes) {
		m_listRecieved = recipes;
		m_selectedId = 0;
	}

	long getSelectedRecipeId() const {
		return m_selectedId;
	}

	void setSelectedRecipeTo(long recipeId) {
		m_selectedId = recipeId;
	}
};

class RecipeListControlTest : public ::testing::Test {
protected:
    vector<Recipe> m_recipesList;
	RecipeListControl* controller;
	StubRecipeListUi stubUi;
	shared_ptr<MockRecipeDAO> m_recipeDao;
	EventManager eventManager;

    virtual void SetUp() {
		controller = new RecipeListControl(eventManager);
		m_recipeDao = shared_ptr<MockRecipeDAO>(new MockRecipeDAO);
		controller->setUi(&stubUi);
		controller->setRecipeDao(m_recipeDao);
        m_recipesList.push_back(createRecipe("r1"));
        m_recipesList.push_back(createRecipe("r2"));

		ON_CALL(*m_recipeDao, getAllRecipeTitles()).WillByDefault(Return(m_recipesList));
    }

	virtual void TearDown() {
		delete controller;
	}

    Recipe createRecipe(QString title) {
		Recipe recipe(0, title);
		return recipe;
	}

	void changeSelectedRecipeTo(long recipeId) {
		eventManager.raiseEvent(shared_ptr<RecipeEvent>(new RecipeEvent(Event::SelectedRecipeChanged, recipeId)));
	}
};

TEST_F(RecipeListControlTest, TestShouldRaiseChangedSelectedRecipeEvent) {
	MockEventHandler eventHandler(eventManager, Event::SelectedRecipeChanged);
	
	long selectedRecipeId = 4;
	dynamic_cast<ISelectionEventListener*>(stubUi.eventListener())->dispatchSelectionChanged(selectedRecipeId);
	EXPECT_EQ(selectedRecipeId, static_pointer_cast<RecipeEvent>(eventHandler._Event)->recipeId());
}

TEST_F(RecipeListControlTest, TestShouldUpdateViewOnRecipeListChangedEvent) {
	eventManager.raiseEvent(shared_ptr<Event>(new Event(Event::RecipeListChanged)));

	vector<Recipe> list = stubUi.m_listRecieved;
	EXPECT_EQ(2, list.size());
	EXPECT_TRUE(find(list.begin(), list.end(), m_recipesList[0]) != list.end());
	EXPECT_TRUE(find(list.begin(), list.end(), m_recipesList[1]) != list.end());
}

TEST_F(RecipeListControlTest, TestShouldUpdateSelectedRecipeOnSelectedRecipeChangedEvent) {
	long newRecipeId = 12;
	changeSelectedRecipeTo(newRecipeId);

	EXPECT_EQ(newRecipeId, stubUi.getSelectedRecipeId());
}

TEST_F(RecipeListControlTest, TestShouldUpdateViewOnRecipeUpdatedEvent) {
	eventManager.raiseEvent(shared_ptr<Event>(new Event(Event::RecipeUpdated)));
	
	vector<Recipe> list = stubUi.m_listRecieved;
	EXPECT_EQ(2, list.size());
	EXPECT_TRUE(find(list.begin(), list.end(), m_recipesList[0]) != list.end());
	EXPECT_TRUE(find(list.begin(), list.end(), m_recipesList[1]) != list.end());
}

TEST_F(RecipeListControlTest, TestShouldKeepSelectedRecipeOnRecipeUpdateEvent) {
	long selectedRecipeId = 12;
	changeSelectedRecipeTo(selectedRecipeId);
	
	eventManager.raiseEvent(shared_ptr<Event>(new Event(Event::RecipeUpdated)));
	EXPECT_EQ(selectedRecipeId, stubUi.m_selectedId);
}

TEST_F(RecipeListControlTest, TestShouldDelegateSearchedRecipesToView) {
	vector<Recipe> recipes = list_of (Recipe(23, "lala")) (Recipe(19, "popo"));
	ON_CALL(*m_recipeDao, getRecipeById(recipes[0].Id)).WillByDefault(Return(recipes[0]));
	ON_CALL(*m_recipeDao, getRecipeById(recipes[1].Id)).WillByDefault(Return(recipes[1]));

	vector<long> idsFound = list_of (recipes[0].Id);
	eventManager.raiseEvent(shared_ptr<RecipeSearchEvent>(new RecipeSearchEvent(idsFound)));
	EXPECT_EQ(1, stubUi.m_listRecieved.size());
	EXPECT_EQ(recipes[0], stubUi.m_listRecieved[0]);
}

TEST_F(RecipeListControlTest, TestShouldDelegateAllRecipeOnClearSearchEvent) {
	eventManager.raiseEvent(shared_ptr<Event>(new Event(Event::RecipeSearchClear)));

	vector<Recipe> list = stubUi.m_listRecieved;
	EXPECT_EQ(2, list.size());
	EXPECT_TRUE(find(list.begin(), list.end(), m_recipesList[0]) != list.end());
	EXPECT_TRUE(find(list.begin(), list.end(), m_recipesList[1]) != list.end());
}

TEST_F(RecipeListControlTest, TestShouldSelectSelectedRecipeWheneverItVisibe) {
	long selectedRecipeId = 2;
	changeSelectedRecipeTo(selectedRecipeId);

	ON_CALL(*m_recipeDao, getRecipeById(m_recipesList[0].Id)).WillByDefault(Return(m_recipesList[0]));

	vector<long> ids = list_of (m_recipesList[0].Id);
	eventManager.raiseEvent(shared_ptr<Event>(new RecipeSearchEvent(ids)));
	eventManager.raiseEvent(shared_ptr<Event>(new Event(Event::RecipeSearchClear)));

	EXPECT_EQ(selectedRecipeId, stubUi.getSelectedRecipeId());
}


}
}

