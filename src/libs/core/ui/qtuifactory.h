#ifndef QTUIFACTORY_H
#define QTUIFACTORY_H
#include <core/ui/uifactory.h>
#include <core/ui/qtmainwindowui.h>

namespace Mamatkon {

class QtUiFactory : public IUiFactory {
    QtMainWindowUi *m_mainWindowUi;
public:
    QtUiFactory() : m_mainWindowUi(new QtMainWindowUi) {}
    ~QtUiFactory() {delete m_mainWindowUi;}
    IMainWindowUi* getMainWindowUi() const;
    IRecipeListUi* getRecipeListUi() const;
	IActionButtonsUi* getActionButtonsUi() const;
	IRecipeGrabberUi* getRecipeGrabberUi() const;
	IRecipeEditUi* getRecipeEditUi() const;
	IFinderUi* getFinderUi() const;
	INutritionalDataUi* getNutritionalDataUi() const;
	void showMainWindow();
};

}
#endif
