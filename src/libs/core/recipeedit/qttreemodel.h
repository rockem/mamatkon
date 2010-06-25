#ifndef QTTREEMODEL_H
#define QTTREEMODEL_H
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <boost/shared_ptr.hpp>

namespace Mamatkon { 

class IngredientItem;

class QtTreeModel : public QAbstractItemModel {
	Q_OBJECT

public:
	QtTreeModel(::boost::shared_ptr<IngredientItem> root, QObject *parent = 0);
	//~QtTreeModel();

	QVariant data(const QModelIndex &index, int role) const;
	bool setData ( const QModelIndex & index, const QVariant & value, int role);
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
		const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
	::boost::shared_ptr<IngredientItem> rootItem;
};

}
#endif // TREEMODEL_H