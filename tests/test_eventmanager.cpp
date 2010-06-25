#include <gmock/gmock.h>
#include <core/eventmanager.h>
#include <boost/bind.hpp>
#include <core/recipeevent.h>

namespace MaTest {
namespace {

using Mamatkon::EventManager;
using Mamatkon::RecipeEvent;
using Mamatkon::Event;
using ::boost::shared_ptr;

class StubEventHandler {
public:
	shared_ptr<Event> eventSent;

	StubEventHandler(EventManager& manager) {
		manager.addEventListener(Event::NewRecipeCreated, METHOD_MEMEBER(StubEventHandler::onEvent));
	}

	void onEvent(shared_ptr<Event> e) {
		eventSent = e;
	}
};

TEST(EventManagetTest, TestShouldRaiseEventByDemand) {
	EventManager em;

	StubEventHandler stubHandler(em);
	em.raiseEvent(shared_ptr<Event>(new Event(Event::NewRecipeCreated)));
	
	EXPECT_TRUE(Event::NewRecipeCreated == stubHandler.eventSent->type());
}

}
}
