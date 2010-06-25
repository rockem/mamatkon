#ifndef QNURITIONALDATAUI_H
#define QNURITIONALDATAUI_H
#include <QWidget>
#include "nutritionaldataui.h"

namespace Ui {
    class QtNutritionalDataUi;
}

namespace Mamatkon {

	class QtNutritionalDataUi : public QWidget, public INutritionalDataUi {
		Q_OBJECT

		QString getValueFrom(const ::std::map<NutritionalData::Types, float>& data, NutritionalData::Types type) const;
	public:
		QtNutritionalDataUi(QWidget *parent = 0);
		~QtNutritionalDataUi();

		void setData(const ::std::map<NutritionalData::Types, float>& data);

	protected:
		void changeEvent(QEvent *e);

	private:
		Ui::QtNutritionalDataUi *m_ui;
	};

}

#endif // QNURITIONALDATAUI_H
