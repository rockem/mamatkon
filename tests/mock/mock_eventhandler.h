#ifndef MOCK_EVENTHANDLER_H
#define MOCK_EVENTHANDLER_H
#include <core/eventmanager.h>
#include <boost/shared_ptr.hpp>

namespace MaTest {

struct MockEventHandler {

	::boost::shared_ptr<Mamatkon::Event> _Event;

	MockEventHandler(Mamatkon::EventManager& em, Mamatkon::Event::Type eventType) {
		em.addEventListener(eventType, METHOD_MEMEBER(MockEventHandler::onEvent));
	}

	void onEvent(::boost::shared_ptr<Mamatkon::Event> e) {
		_Event = e;
	}
};

}

#endif // MOCK_EVENTHANDLER_H
