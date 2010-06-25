#include <core/ui/qtuifactory.h>
#include <core/recipelist/qtrecipelistui.h>
#include <core/ui/mainwindowui.h>
#include <core/actionbuttons/qtactionbuttonsui.h>
#include <core/recipegrabber/qtrecipegrabberui.h>
#include <core/recipeedit/qtrecipeeditui.h>
#include <core/finder/qtfinderui.h>
#include <core/nuritionaldata/qtnutritionaldataui.h>

namespace Mamatkon {

IRecipeListUi* QtUiFactory::getRecipeListUi() const {
    return m_mainWindowUi->recipeListUi();
}

IMainWindowUi* QtUiFactory::getMainWindowUi() const {
    return m_mainWindowUi;
}

IActionButtonsUi* QtUiFactory::getActionButtonsUi() const {
	return m_mainWindowUi->actionButtonsUi();
}

void QtUiFactory::showMainWindow() {
	m_mainWindowUi->show();
}

IRecipeGrabberUi* QtUiFactory::getRecipeGrabberUi() const {
	return m_mainWindowUi->recipeGrabberUi();
}

IRecipeEditUi* QtUiFactory::getRecipeEditUi() const {
	return m_mainWindowUi->recipeEditUi();
}

IFinderUi* QtUiFactory::getFinderUi() const {
	return m_mainWindowUi->finderUi();
}

INutritionalDataUi* QtUiFactory::getNutritionalDataUi() const {
	return m_mainWindowUi->nutritionalDataUi();
}

}
