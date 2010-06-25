#ifndef _QTMAINWINDOWUI_H
#define _QTMAINWINDOWUI_H
#include <QtGui/QMainWindow>
#include <core/ui/mainwindowui.h>

namespace Ui {
    class QtMainWindowUi;
}

namespace Mamatkon {

class QtRecipeListUi;
class QtActionButtonsUi;
class QtNotificationAreaUi;
class QtImportRecipe;
class QtRecipeGrabberUi;
class QtRecipeEditUi;
class QtAboutUi;
class QtFinderUi;
class QtNutritionalDataUi;

class QtMainWindowUi : public QMainWindow, public IMainWindowUi {
    Q_OBJECT
public:
    QtMainWindowUi(QWidget *parent = 0);
    ~QtMainWindowUi();

     QtRecipeListUi* recipeListUi() const;
	 QtActionButtonsUi* actionButtonsUi() const;
	 QtRecipeGrabberUi* recipeGrabberUi() const;
	 QtRecipeEditUi* recipeEditUi() const;
	 QtFinderUi* finderUi() const;
	 QtNutritionalDataUi* nutritionalDataUi() const;
protected:
	void changeEvent(QEvent *e);

private slots:
	void showAboutDialog();

private:
	void changeTabsAccordingtoLayout();
	
    Ui::QtMainWindowUi *m_ui;
	QtAboutUi* m_aboutDialog;
};

}

#endif // QTMAINWINDOWUI_H
