#include <core/ui/qtmainwindowui.h>
using Mamatkon::QtRecipeListUi;
using Mamatkon::QtActionButtonsUi;
using Mamatkon::QtNotificationAreaUi;
using Mamatkon::QtImportRecipe;
using Mamatkon::QtRecipeGrabberUi;
using Mamatkon::QtRecipeEditUi;
using Mamatkon::QtFinderUi;
using Mamatkon::QtNutritionalDataUi;
#include "ui_qtmainwindowui.h"
#include <core/ui/qtaboutui.h>

namespace Mamatkon {

QtMainWindowUi::QtMainWindowUi(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::QtMainWindowUi),
	m_aboutDialog(0) {
    m_ui->setupUi(this);
	setLayoutDirection(QApplication::layoutDirection());
	changeTabsAccordingtoLayout();
	connect(m_ui->fileItemExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(m_ui->helpItemAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
}

QtMainWindowUi::~QtMainWindowUi() {
    delete m_ui;
	delete m_aboutDialog;
}

QtRecipeListUi* QtMainWindowUi::recipeListUi() const {
    return m_ui->recipeListUi;
}

QtActionButtonsUi* QtMainWindowUi::actionButtonsUi() const {
	return m_ui->actionButtonsUi;
}

void QtMainWindowUi::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void QtMainWindowUi::changeTabsAccordingtoLayout() {
	QTabWidget::TabPosition pos;
	if(QApplication::layoutDirection() == Qt::LeftToRight) {
		pos = QTabWidget::West;
	} else {
		pos = QTabWidget::East;
	}
	m_ui->recipeRelated->setTabPosition(pos);
}

QtRecipeGrabberUi* QtMainWindowUi::recipeGrabberUi() const {
	return m_ui->recipeGrabber;
}

QtRecipeEditUi* QtMainWindowUi::recipeEditUi() const {
	return m_ui->recipeEdit;
}

void QtMainWindowUi::showAboutDialog() {
	if(m_aboutDialog == 0) {
		m_aboutDialog = new QtAboutUi(this);
	}
	m_aboutDialog->show();
	m_aboutDialog->raise();
	m_aboutDialog->activateWindow();
}

QtFinderUi* QtMainWindowUi::finderUi() const {
	return m_ui->finderUi;
}

QtNutritionalDataUi* QtMainWindowUi::nutritionalDataUi() const {
	return m_ui->nutritionalDataUi;	
}
}




