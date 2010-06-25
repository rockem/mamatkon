#ifndef RECIPEEVENT_H
#define RECIPEEVENT_H
#include <core/event.h>

namespace Mamatkon {

class RecipeEvent : public Event {
	long m_id;
public:
	RecipeEvent(Type type, long recipeId) : Event(type), m_id(recipeId) {}

	long recipeId() const { return m_id; }

};

}
#endif // RECIPEEVENT_H
