#include "ingredientitem.h"
#include <QStringList>

namespace Mamatkon {

IngredientItem::IngredientItem(const QList<QVariant> &data, long id, IngredientItem *parent) : m_id(id) {
	parentItem = parent;
	itemData = data;
}

IngredientItem::~IngredientItem() {
	qDeleteAll(childItems);
}

void IngredientItem::appendChild(IngredientItem *item) {
	childItems.append(item);
}

IngredientItem *IngredientItem::child(int row) {
	return childItems.value(row);
}

int IngredientItem::childCount() const {
	return childItems.count();
}

int IngredientItem::columnCount() const {
	return itemData.count();
}

QVariant IngredientItem::data(int column) const {
	return itemData.value(column);
}

IngredientItem *IngredientItem::parent() {
	return parentItem;
}

int IngredientItem::row() const {
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<IngredientItem*>(this));

	return 0;
}

long IngredientItem::id() const {
	return m_id;
}

void IngredientItem::setData( int column, QVariant value ) {
	itemData[column] = value;
}
}