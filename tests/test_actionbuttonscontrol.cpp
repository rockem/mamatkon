#include <gmock/gmock.h>
#include <core/actionbuttons/actionbuttonscontrol.h>
#include <core/actionbuttons/actionbuttonsui.h>
#include <core/eventlistener.h>
#include <boost/shared_ptr.hpp>
#include <core/eventmanager.h>
#include "mock/mock_recipedao.h"
#include <core/recipeevent.h>

namespace MaTest {
namespace {

using ::boost::shared_ptr;
using Mamatkon::ActionButtonsControl;
using Mamatkon::IActionButtonsUi;
using Mamatkon::IActionButtonsEventListener;
using Mamatkon::CommandCommander;
using Mamatkon::EventManager;
using Mamatkon::Event;
using ::testing::Return;
using ::boost::static_pointer_cast;
using Mamatkon::RecipeEvent;

class ActionButtonsControlTest : public ::testing::Test {
protected:
	EventManager eventManager;
	ActionButtonsControl* m_control;
	shared_ptr<Event> newRecipeEvent;

	ActionButtonsControlTest() {
		m_control = new ActionButtonsControl(eventManager);
	}

	~ActionButtonsControlTest() {
		delete m_control;
	}

public:
	void onNewRecipe(shared_ptr<Event> e) {
		newRecipeEvent = e;
	}
};

//TEST_F(ActionButtonsControlTest, testShouldRunPublishRecipeToCommand) {
//	m_commander->registerCommand(Command::PublishSelectedRecipe, m_command);
//    m_control.dispatchClick(4);
//}

TEST_F(ActionButtonsControlTest, testShouldCreateNewRecipeAndRaiseEvent) {
	shared_ptr<MockRecipeDAO> dao = shared_ptr<MockRecipeDAO>(new MockRecipeDAO);
	m_control->setRecipeDao(dao);
	eventManager.addEventListener(Event::NewRecipeCreated, ::boost::bind(&ActionButtonsControlTest::onNewRecipe, this, _1));

	long newRecipeId = 13;
	EXPECT_CALL(*dao, createNewRecipe()).WillOnce(Return(newRecipeId));

	m_control->dispatchClick(1);
	EXPECT_EQ(newRecipeId, static_pointer_cast<RecipeEvent>(newRecipeEvent)->recipeId());
}

}
}

