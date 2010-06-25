#ifndef UIBASE_H
#define UIBASE_H
#include <core/eventlistener.h>

namespace Mamatkon {

class IUi {
public:
	virtual ~IUi() {}
	virtual void setEventListener(IEventListener* listener) = 0;
	virtual IEventListener* eventListener() const = 0;
};

class UiBase : public IUi {
	IEventListener* m_eventListener;
public:
	virtual ~UiBase() {}
	virtual void setEventListener(IEventListener* listener) {m_eventListener = listener;}
	virtual IEventListener* eventListener() const {return m_eventListener;}

};

}
#endif // UIBASE_H
