#include <gmock/gmock.h>
#include <core/recipegrabber/recipegrabbercontrol.h>
#include <core/eventmanager.h>
#include <QString>
#include <core/db/recipe.h>
#include "mock/mock_recipedao.h"
#include <core/eventlistener.h>
#include "mock/mock_eventhandler.h"
#include <boost/pointer_cast.hpp>
#include <core/recipeevent.h>


namespace MaTest {
namespace {

using Mamatkon::EventManager;
using Mamatkon::RecipeGrabberControl;
using Mamatkon::Recipe;
using Mamatkon::Event;
using Mamatkon::IRecipeGrabberEventListener;
using ::boost::shared_ptr;
using ::testing::Return;
using ::boost::static_pointer_cast;
using Mamatkon::RecipeEvent;

class RecipeGrabberControlTest : public ::testing::Test {
protected:
	EventManager eventManager;
	RecipeGrabberControl* control;
	shared_ptr<MockRecipeDAO> dao;
	Recipe recipe;
	MockEventHandler* eventHandler;

	RecipeGrabberControlTest() : recipe(Recipe(14, "popo")){
		control = new RecipeGrabberControl(eventManager);
		dao = shared_ptr<MockRecipeDAO>(new MockRecipeDAO);
		control->setRecipeDao(dao);

		EXPECT_CALL(*dao, getRecipeById(recipe.Id)).WillOnce(Return(recipe));

		eventManager.raiseEvent(shared_ptr<RecipeEvent>(new RecipeEvent(Event::SelectedRecipeChanged, recipe.Id)));

		eventHandler = new MockEventHandler(eventManager, Event::RecipeUpdated);
	}

	~RecipeGrabberControlTest() {
		EXPECT_TRUE(recipe.Id == static_pointer_cast<RecipeEvent>(eventHandler->_Event)->recipeId());
		delete control;
		delete eventHandler;
	}

	void setUpExpectations() {
		EXPECT_CALL(*dao, updateRecipe(recipe));
	}
};

TEST_F(RecipeGrabberControlTest, TestShouldUpdateCurrentRecipeWithSelectedAsTitle) {
	recipe.Title = "spopov";
	setUpExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Title, recipe.Title);
}

TEST_F(RecipeGrabberControlTest, TestShouldUpdateCurrentRecipeWithSelectedAsInstructions) {
	recipe.Instruction = "lalalal ";
	setUpExpectations();
	
	control->dispatchGrab(IRecipeGrabberEventListener::Instructions, recipe.Instruction);
}

TEST_F(RecipeGrabberControlTest, TestShouldUpdateCurrentRecipeWithSelectedAsSource) {
	recipe.Source = "mama";
	setUpExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Source, recipe.Source);
}

TEST_F(RecipeGrabberControlTest, TestShouldUpdateCurrentRecipeWithSelectedAsServings) {
	recipe.Servings = 4;
	setUpExpectations();

	control->dispatchGrab(IRecipeGrabberEventListener::Servings, "4");
}

}
}
