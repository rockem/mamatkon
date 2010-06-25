#ifndef QTNUMBERCOLUMNDELEGATE_H
#define QTNUMBERCOLUMNDELEGATE_H
#include <QItemDelegate>
#include <QWidget>
#include <QStyledItemDelegate>

namespace Mamatkon { 

class AmountColumnDelegate : public  QStyledItemDelegate {
	Q_OBJECT

public:
	AmountColumnDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

	void paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;
};

}
#endif // QTNUMBERCOLUMNDELEGATE_H