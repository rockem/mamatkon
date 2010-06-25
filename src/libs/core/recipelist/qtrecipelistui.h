#ifndef QTRECIPELISTUI_H
#define QTRECIPELISTUI_H
#include <QtGui/QWidget>
#include <core/recipelist/recipelistui.h>
#include <QtGui/QListWidgetItem>


namespace Ui {
    class QtRecipeListUi;
}

namespace Mamatkon {

class QtRecipeListUi : public QWidget, public IRecipeListUi {
    Q_OBJECT


	class ItemWidget : public QListWidgetItem {
		long m_id;

	public:
		ItemWidget() : m_id(0) {}
		void setId(long id) {m_id = id; }
		long id() const {return m_id;}
	};

public:
    QtRecipeListUi(QWidget *parent = 0);
    ~QtRecipeListUi();

    void updateRecipeList(const std::vector<Recipe>& recipes);
	long getSelectedRecipeId() const;
	void setSelectedRecipeTo(long recipeId);

private slots:
	void onSelectionChange();

private:
    Ui::QtRecipeListUi *m_ui;
};

}

#endif // QTRECIPELISTUI_H
