#ifndef QTFINDERUI_H
#define QTFINDERUI_H
#include <QWidget>
#include "finderui.h"
#include <map>
#include "recipefinder.h"

namespace Ui {
    class QtFinderUi;
}

namespace Mamatkon {

	class QtFinderUi : public QWidget, public IFinderUi {
		Q_OBJECT

			static ::std::map<int, IRecipeFinder::SearchType> m_intToSearchType;
	public:
		QtFinderUi(QWidget *parent = 0);
		~QtFinderUi();

	protected:
		void changeEvent(QEvent *e);

	private slots:
		void onFind();
		void onClear();


	private:
		Ui::QtFinderUi *m_ui;
	};

}

#endif // QTFINDERUI_H
