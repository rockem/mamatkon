#ifndef QTABOUTUI_H
#define QTABOUTUI_H

#include <QDialog>

namespace Ui {
    class QtAboutUi;
}

namespace Mamatkon {

	class QtAboutUi : public QDialog {
		Q_OBJECT
	public:
		QtAboutUi(QWidget *parent = 0);
		~QtAboutUi();

	protected:
		void changeEvent(QEvent *e);

	private:
		Ui::QtAboutUi *ui;
	};
}

#endif // QTABOUTUI_H
