#include "qtrecipelistui.h"
#include <core/db/recipe.h>
#include "ui_qtrecipelistui.h"
#include <core/eventlistener.h>

namespace Mamatkon {

using ::boost::shared_ptr;
using ::std::vector;

QtRecipeListUi::QtRecipeListUi(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::QtRecipeListUi) {
    m_ui->setupUi(this);

	QObject::connect(m_ui->recipeList, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectionChange()));
}

QtRecipeListUi::~QtRecipeListUi() {
    delete m_ui;
}

void QtRecipeListUi::updateRecipeList(const vector<Recipe>& recipes) {
    m_ui->recipeList->clear();
    vector<Recipe>::const_iterator it = recipes.begin();
    for (; it != recipes.end(); ++it) {
		ItemWidget* item = new ItemWidget;
		item->setText(it->Title);
		item->setId(it->Id);
        m_ui->recipeList->addItem(item);
    }
}

long QtRecipeListUi::getSelectedRecipeId() const {
	ItemWidget* item = dynamic_cast<ItemWidget*>(m_ui->recipeList->currentItem());
	if(item == 0) {
		return -1;
	}
	return item->id();
}

void QtRecipeListUi::setSelectedRecipeTo(long recipeId) {
	QList<QListWidgetItem*> allItems = m_ui->recipeList->findItems("*", Qt::MatchWildcard);
	for (int i = 0; i < allItems.size(); ++i) {
		if(dynamic_cast<ItemWidget*>(allItems.at(i))->id() == recipeId) {
			m_ui->recipeList->setCurrentItem(allItems.at(i));
			break;
		}
	}
}

void QtRecipeListUi::onSelectionChange() {
	ItemWidget* item = dynamic_cast<ItemWidget*>(m_ui->recipeList->currentItem());
	if(item != 0) {
		dynamic_cast<ISelectionEventListener*>(eventListener())->dispatchSelectionChanged(item->id());
	}
}
}
