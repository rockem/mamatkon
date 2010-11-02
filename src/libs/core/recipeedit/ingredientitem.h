#ifndef INGREDIENTITEM_H
#define INGREDIENTITEM_H
#include <QList>
#include <QVariant>

namespace Mamatkon { 

class IngredientItem {
public:
	IngredientItem(const QList<QVariant> &data, long id = 0,  IngredientItem *parent = 0);
	~IngredientItem();

	void appendChild(IngredientItem *child);

	long id() const;
	IngredientItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	void setData(int column, QVariant value);
	int row() const;
	IngredientItem *parent();

private:
	long m_id;
	QList<IngredientItem*> childItems;
	QList<QVariant> itemData;
	IngredientItem *parentItem;
};

}
#endif // INGREDIENTITEM_H
