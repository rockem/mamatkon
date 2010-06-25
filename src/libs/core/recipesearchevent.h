#ifndef RECIPESEARCHEVENT_H
#define RECIPESEARCHEVENT_H
#include <core/event.h>
#include <vector>

namespace Mamatkon { 

	class RecipeSearchEvent : public Event {
		::std::vector<long> m_recipeIds;
	public:
		RecipeSearchEvent(::std::vector<long> ids) : Event(Event::RecipeSearch), m_recipeIds(ids) {}

		::std::vector<long> recipeIds() const { return m_recipeIds; }
	};

}
#endif // RECIPESEARCHEVENT_H