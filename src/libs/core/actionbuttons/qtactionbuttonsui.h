#ifndef QTACTIONBUTTONSUI_H
#define QTACTIONBUTTONSUI_H
#include <QtGui/QGroupBox>
#include <core/actionbuttons/actionbuttonsui.h>

namespace Ui {
    class QtActionButtonsUi;
}

namespace Mamatkon {
struct IActionButtonsEventListener;

class QtActionButtonsUi : public QGroupBox, public IActionButtonsUi {
    Q_OBJECT

public:
    QtActionButtonsUi(QWidget *parent = 0);
    ~QtActionButtonsUi();

private:
    Ui::QtActionButtonsUi *m_ui;
private slots:
    void dispatchClick();
    void trig(QAction* a);
	void paste();

};

}

#endif // QTACTIONBUTTONSUI_H
