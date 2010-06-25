#include "qtnotificationareaui.h"
#include "ui_qtnotificationareaui.h"

namespace Mamatkon {

QtNotificationAreaUi::QtNotificationAreaUi(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::QtNotificationAreaUi) {
    m_ui->setupUi(this);
}

QtNotificationAreaUi::~QtNotificationAreaUi() {
    delete m_ui;
}

void QtNotificationAreaUi::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

}
