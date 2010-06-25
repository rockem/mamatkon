#include "qttreemodel.h"
#include "ingredientitem.h"
#include <QtGui>

namespace Mamatkon {

using ::boost::shared_ptr;

QtTreeModel::QtTreeModel(::boost::shared_ptr<IngredientItem> root, QObject *parent)
	: QAbstractItemModel(parent) {
	rootItem = root;
}

int QtTreeModel::columnCount(const QModelIndex &parent) const {
	if (parent.isValid()) {
		return static_cast<IngredientItem*>(parent.internalPointer())->columnCount();
	} else {
		return rootItem->columnCount();
	}
}

QVariant QtTreeModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid()) {
		return QVariant();
	}

	if (role != Qt::DisplayRole && role != Qt::EditRole) {
		return QVariant();
	}

	IngredientItem *item = static_cast<IngredientItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags QtTreeModel::flags(const QModelIndex &index) const {
	if (!index.isValid()) {
		return 0;
	}
	if(index.column() == 4) {
		return Qt::ItemIsSelectable;
	}

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant QtTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		QString s = rootItem->data(section).toString();
		return tr(s.toLocal8Bit().constData());
	}

	return QVariant();
}

QModelIndex QtTreeModel::index(int row, int column, const QModelIndex &parent) const {
	if (!hasIndex(row, column, parent)) {
		return QModelIndex();
	}

	IngredientItem* parentItem;

	if (!parent.isValid()) {
		parentItem = rootItem.get();
	} else {
		parentItem = static_cast<IngredientItem*>(parent.internalPointer());
	}

	IngredientItem *childItem = parentItem->child(row);
	if (childItem) {
		return createIndex(row, column, childItem);
	} else {
		return QModelIndex();
	}
}

QModelIndex QtTreeModel::parent(const QModelIndex &index) const {
	if (!index.isValid()) {
		return QModelIndex();
	}

	IngredientItem *childItem = static_cast<IngredientItem*>(index.internalPointer());
	IngredientItem *parentItem = childItem->parent();

	if (parentItem == rootItem.get()) {
		return QModelIndex();
	}

	return createIndex(parentItem->row(), 0, parentItem);
}

int QtTreeModel::rowCount(const QModelIndex &parent) const {
	IngredientItem *parentItem;
	if (parent.column() > 0) {
		return 0;
	}

	if (!parent.isValid()) {
		parentItem = rootItem.get();
	} else {
		parentItem = static_cast<IngredientItem*>(parent.internalPointer());
	}

	return parentItem->childCount();
}

bool QtTreeModel::setData(const QModelIndex & index, const QVariant & value, int role) {
	IngredientItem *item = static_cast<IngredientItem*>(index.internalPointer());
	item->setData(index.column(), value);
	emit dataChanged(index, index);
	return true;
}
}