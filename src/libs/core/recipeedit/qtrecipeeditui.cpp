#include "qtrecipeeditui.h"
#include "ui_qtrecipeeditui.h"
#include "qttreemodel.h"
#include "ingredientitem.h"
#include <core/recipeupdater.h>
#include "qtnumbercolumndelegate.h"
#include <QModelIndex>


using namespace Mamatkon;

QtRecipeEditUi::QtRecipeEditUi(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::QtRecipeEditUi) {
    m_ui->setupUi(this);

	setLayoutDirection(QApplication::layoutDirection());
	m_ui->ingredientsList->setItemDelegateForColumn(0, new AmountColumnDelegate);
	m_ui->ingredientsList->setItemDelegateForColumn(4, new AmountColumnDelegate);
	connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(onFocusChanged(QWidget*, QWidget*)));
	connect(m_ui->deleteBtn, SIGNAL(clicked()), this, SLOT(onDeleteIngredient()));
}

QtRecipeEditUi::~QtRecipeEditUi() {
    delete m_ui;
}

void QtRecipeEditUi::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QtRecipeEditUi::updateRecipeFields(Recipe recipe) {
	m_ui->m_recipeTitle->setText(recipe.Title);
	m_ui->m_instructions->setText(recipe.Instruction);
	m_ui->m_source->setText(recipe.Source);
	m_ui->m_servings->setText(QString::number(recipe.Servings));
	m_ui->m_scaledServings->setText(QString::number(recipe.ServingsScale));
	m_ui->m_tagsTxt->setText(recipe.Tags);

}

void QtRecipeEditUi::updateIngredients( ::boost::shared_ptr<IngredientItem> ingredients) {
	QtTreeModel* model = new QtTreeModel(ingredients);
	QAbstractItemModel* oldModel = m_ui->ingredientsList->model();
	m_ui->ingredientsList->setModel(model);
	delete oldModel;
	connect(model, 
		SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), 
		this, 
		SLOT(onIngredientListChange(const QModelIndex&, const QModelIndex&)));
}

void QtRecipeEditUi::onFocusChanged(QWidget* old, QWidget* now)  {
	if(old == m_ui->m_recipeTitle ||
		old == m_ui->m_instructions ||
		old == m_ui->m_servings ||
		old == m_ui->m_scaledServings ||
		old == m_ui->m_source ||
		old == m_ui->m_tagsTxt) {
		dynamic_cast<IRecipeUpdater*>(eventListener())->dispatchRecipeUpdate(createRecipeFromFields());
	}
}

Recipe QtRecipeEditUi::createRecipeFromFields() {
	Recipe recipe(0, m_ui->m_recipeTitle->text());
	recipe.Instruction = m_ui->m_instructions->toPlainText();
	recipe.Source = m_ui->m_source->text();
	recipe.Servings = m_ui->m_servings->text().toInt();
	recipe.ServingsScale = m_ui->m_scaledServings->text().toInt();
	recipe.Tags = m_ui->m_tagsTxt->text();
	return recipe;
}

void QtRecipeEditUi::onDeleteIngredient() {
	QItemSelectionModel* sel = m_ui->ingredientsList->selectionModel();
	IngredientItem* item = static_cast<IngredientItem*>(sel->currentIndex().internalPointer());
	dynamic_cast<IRecipeUpdater*>(eventListener())->dispatchDeleteIngredient(item->id());
}

void Mamatkon::QtRecipeEditUi::onIngredientListChange( const QModelIndex& topLeft, const QModelIndex& bottomRight ) {
	QItemSelectionModel* sel = m_ui->ingredientsList->selectionModel();
	IngredientItem* item = static_cast<IngredientItem*>(sel->currentIndex().internalPointer());
	dynamic_cast<IRecipeUpdater*>(eventListener())->dispatchIngredientsUpdate(item);
}