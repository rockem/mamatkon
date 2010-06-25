#ifndef QTNOTIFICATIONAREAUI_H
#define QTNOTIFICATIONAREAUI_H
#include <QtGui/QWidget>

namespace Ui {
    class QtNotificationAreaUi;
}

namespace Mamatkon {

class QtNotificationAreaUi : public QWidget {
    Q_OBJECT
public:
    QtNotificationAreaUi(QWidget *parent = 0);
    ~QtNotificationAreaUi();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QtNotificationAreaUi *m_ui;
};

}

#endif // QTNOTIFICATIONAREAUI_H
