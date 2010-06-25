#ifndef EVENT_H
#define EVENT_H

namespace Mamatkon {

class Event {
public:
	enum Type {
		NewRecipeCreated, 
		SelectedRecipeChanged, 
		RecipeListChanged, 
		RecipeUpdated, 
		RecipeSearch, 
		RecipeSearchClear};

	Type type() const {	return m_type; }

	Event(Type type) : m_type(type) {}
private:
	Type m_type;
};

}

#endif // EVENT_H
