#ifndef UI_FACTORY_H
#define UI_FACTORY_H
#include <core/recipelist/recipelistui.h>
#include <core/ui/mainwindowui.h>
#include <core/actionbuttons/actionbuttonsui.h>
#include <core/recipegrabber/recipegrabberui.h>
#include <core/recipeedit/recipeeditui.h>
#include <core/finder/finderui.h>
#include <core/nuritionaldata/nutritionaldataui.h>

namespace Mamatkon {

struct IUiFactory {
    virtual ~IUiFactory() {}
    virtual IRecipeListUi* getRecipeListUi() const = 0;
    virtual IMainWindowUi* getMainWindowUi() const = 0;
    virtual IActionButtonsUi* getActionButtonsUi() const = 0;
	virtual IRecipeGrabberUi* getRecipeGrabberUi() const = 0;
	virtual IRecipeEditUi* getRecipeEditUi() const = 0;
	virtual IFinderUi* getFinderUi() const = 0;
	virtual INutritionalDataUi* getNutritionalDataUi() const = 0;
	virtual void showMainWindow() = 0;
};

}
#endif
