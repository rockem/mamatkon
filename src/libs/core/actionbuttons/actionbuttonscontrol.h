#ifndef ACTIONBUTTONSCONTROL_H
#define ACTIONBUTTONSCONTROL_H
#include <core/eventmanager.h>
#include <core/eventlistener.h>
#include <core/controlbase.h>
#include <map>

namespace Mamatkon {

    struct IUiFactory;
    struct IActionButtonsUi;
    class IRecipeDAO;

class ActionButtonsControl : public ControlBase, public IActionButtonsEventListener {
	::boost::shared_ptr<IRecipeDAO> m_recipeDao;

    long m_currentRecipeId;
    void onRecipeSelectionChanged(::boost::shared_ptr<Event> e) ;
public:
	ActionButtonsControl(EventManager& em);

	void dispatchClick(int buttonNumber) const;
};

}
#endif // ACTIONBUTTONSCONTROL_H
