#include "eventmanager.h"


namespace Mamatkon {

using ::std::map;
using ::std::string;
using ::std::list;
using ::boost::function;
using ::boost::shared_ptr;

void EventManager::
addEventListener(Event::Type eventType, ::boost::function<void(::boost::shared_ptr<Event>)> method) {
	m_listenersMap[eventType].push_back(method);
}

void EventManager::raiseEvent(::boost::shared_ptr<Event> e) {
	list<function<void(shared_ptr<Event>)> >::iterator it = m_listenersMap[e->type()].begin();
	for(; it != m_listenersMap[e->type()].end(); ++it) {
		(*it)(e);
	}
}

}
