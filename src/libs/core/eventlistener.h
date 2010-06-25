#ifndef _IEVENTLISTENER_H
#define _IEVENTLISTENER_H

class QString;

namespace Mamatkon {

struct IEventListener {
	virtual ~IEventListener() {}
};

struct IActionButtonsEventListener : public IEventListener {
	virtual void dispatchClick(int buttonNumber) const = 0;
};

class TargetEventListener {
	IEventListener* m_eventListener;
public:
	virtual ~TargetEventListener() {}
	virtual void setEventListener(IEventListener* listener) {m_eventListener = listener;}
	virtual IEventListener* eventListener() const {return m_eventListener;}
};

struct ISelectionEventListener : public IEventListener {
	virtual ~ISelectionEventListener() {}
	virtual void dispatchSelectionChanged(long recipeId) = 0;
};

class IRecipeGrabberEventListener : public IEventListener {
public:
	enum RecipeSection {Title, Ingredients, Instructions, Source, Servings};
	virtual ~IRecipeGrabberEventListener() {}
	virtual void dispatchGrab(RecipeSection section, const QString& text) = 0;
};

}


#endif
