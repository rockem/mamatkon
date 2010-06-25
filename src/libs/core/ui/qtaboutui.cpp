#include "qtaboutui.h"
#include "ui_qtaboutui.h"
#include <QPixmap>

using namespace Mamatkon;

QtAboutUi::QtAboutUi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtAboutUi) {
    ui->setupUi(this);

    ui->waterMark->setPixmap(QPixmap(QLatin1String(":/image/images/mamatkon2.png")));
    connect(ui->closeBtn, SIGNAL(clicked()), this, SLOT(close()));
}

QtAboutUi::~QtAboutUi() {
    delete ui;
}

void QtAboutUi::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
