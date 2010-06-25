#ifndef MOCK_UIFACTORY_H
#define MOCK_UIFACTORY_H
#include <core/ui/uifactory.h>

namespace Mamatkon {
	struct IRecipeListUi;
	class IMainWindowUi;
	struct IActionButtonsUi;
}

namespace MaTest {

class MockRecipeListUi;

class MockUiFactory : public Mamatkon::IUiFactory {
public:
	Mamatkon::IRecipeListUi* m_recipeListUi;
    Mamatkon::IActionButtonsUi* m_actionButtonsUi;

	Mamatkon::IRecipeListUi* getRecipeListUi() const { return m_recipeListUi; }
	Mamatkon::IMainWindowUi* getMainWindowUi() const { return static_cast<Mamatkon::IMainWindowUi*>(0); }
	Mamatkon::IActionButtonsUi* getActionButtonsUi() const { return m_actionButtonsUi; }
	Mamatkon::IRecipeGrabberUi* getRecipeGrabberUi() const { return static_cast<Mamatkon::IRecipeGrabberUi*>(0); }
	void showMainWindow() {}
};

}

#endif

