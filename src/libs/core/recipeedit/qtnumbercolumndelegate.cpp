#include "qtnumbercolumndelegate.h"
#include <QStyledItemDelegate>

namespace Mamatkon {


void AmountColumnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
	const QModelIndex &index) const {
		QStyleOptionViewItem ltrOption(option);
		ltrOption.direction = Qt::LeftToRight;
		if(option.direction == Qt::LeftToRight) {
			ltrOption.displayAlignment = Qt::AlignLeft;
		} else {
			ltrOption.displayAlignment = Qt::AlignRight;
		}
		QStyledItemDelegate::paint(painter, ltrOption, index);
}

}