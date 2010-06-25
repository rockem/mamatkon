#ifndef GRABRECIPEDETAILS_H
#define GRABRECIPEDETAILS_H
#include <QtGui/QWidget>
#include <core/ui/uibase.h>
#include "recipegrabberui.h"

namespace Ui {
    class QtRecipeGrabberUi;
}

namespace Mamatkon {


class QtRecipeGrabberUi : public QWidget, public IRecipeGrabberUi {
    Q_OBJECT
public:
    QtRecipeGrabberUi(QWidget *parent = 0);
	~QtRecipeGrabberUi();

protected:
    void changeEvent(QEvent *e);

private slots:
	void grabTitle();
	void grabIngredients();
	void grabInstructions();
	void grabSource();
	void grabServings();

private:
	void dispatchGrabFor(IRecipeGrabberEventListener::RecipeSection section);
	void modifyTextEditForRTLSupport();

    Ui::QtRecipeGrabberUi *m_ui;
};

}

#endif // GRABRECIPEDETAILS_H
