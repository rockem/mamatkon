#ifndef QTRECIPEEDITUI_H
#define QTRECIPEEDITUI_H
#include <QWidget>
#include <core/recipeedit/recipeeditui.h>
#include <core/db/recipe.h>

class QModelIndex;

namespace Ui {
    class QtRecipeEditUi;
}

namespace Mamatkon {

class QtRecipeEditUi : public QWidget, public IRecipeEditUi {
    Q_OBJECT
public:
    QtRecipeEditUi(QWidget *parent = 0);
    ~QtRecipeEditUi();

	void updateRecipeFields(Recipe recipe);
	void updateIngredients(::boost::shared_ptr<IngredientItem> ingredients);
	Recipe createRecipeFromFields();
protected:
    void changeEvent(QEvent *e);

private slots:
	void onFocusChanged(QWidget*, QWidget*);
	void onDeleteIngredient();
	void onIngredientListChange(const QModelIndex& topLeft, const QModelIndex& bottomRight);

private:
    Ui::QtRecipeEditUi *m_ui;
};

}

#endif // QTRECIPEEDITUI_H
