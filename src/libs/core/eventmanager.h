#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <map>
#include <string>
#include <list>
#include <core/event.h>
#include <boost/shared_ptr.hpp>

namespace Mamatkon {

#define METHOD_MEMEBER(method) boost::bind(&method, this, _1)

class EventManager {
	::std::map<Event::Type, ::std::list<boost::function<void(::boost::shared_ptr<Event>)> > > m_listenersMap;

    void onRecipeSelectionChanged(::boost::shared_ptr<Event> e);
public:
	void addEventListener(Event::Type eventType, ::boost::function<void(::boost::shared_ptr<Event>)> aa);
	void raiseEvent(::boost::shared_ptr<Event> e);
};
}
#endif // EVENTMANAGER_H
