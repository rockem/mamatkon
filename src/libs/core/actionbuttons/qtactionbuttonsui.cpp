#include "qtactionbuttonsui.h"
#include <core/eventlistener.h>
#include <QMetaMethod>
#include "ui_qtactionbuttonsui.h"

namespace Mamatkon {

QtActionButtonsUi::QtActionButtonsUi(QWidget *parent) :
    QGroupBox(parent),
    m_ui(new Ui::QtActionButtonsUi) {
    m_ui->setupUi(this);

    m_ui->actionButton1->setDefaultAction(m_ui->actionNewRecipe);
    m_ui->actionNewRecipe->setData(QVariant(1));
    m_ui->actionButton4->setDefaultAction(m_ui->actionPublish);
    m_ui->actionPublish->setData(QVariant(4));
    QObject::connect(m_ui->actionButton1, SIGNAL(triggered(QAction*)), this, SLOT(trig(QAction*)));
    QObject::connect(m_ui->actionButton4, SIGNAL(triggered(QAction*)), this, SLOT(trig(QAction*)));
	QObject::connect(m_ui->actionButton2, SIGNAL(clicked()), this, SLOT(paste()));
}

QtActionButtonsUi::~QtActionButtonsUi() {
    delete m_ui;
}

void QtActionButtonsUi::dispatchClick() {
    // dynamic_cast<IActionButtonsEventListener*>(eventListener())->dispatchClick(4);
}

void QtActionButtonsUi::trig(QAction* action) {
    dynamic_cast<IActionButtonsEventListener*>(eventListener())->dispatchClick(action->data().Int);
}

void QtActionButtonsUi::paste() {
	QWidget* w = QApplication::focusWidget();
	const QMetaObject* mo = w->metaObject();
	int mi = mo->indexOfSlot("paste()");
	if(mi != -1) {
		QMetaMethod mm = w->metaObject()->method(mi);
		mm.invoke(w);
	}
}


}
