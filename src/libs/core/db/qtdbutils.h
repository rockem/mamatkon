#ifndef _QTDBUTILS_H
#define _QTDBUTILS_H

class QSqlDatabase;
class QSqlQuery;
class QString;
class QStringList;

namespace Mamatkon {

class QtIngredientDao;

class QtDbUtils {
	static const QString k_CreateRecipeTable;
	static const QString k_CreateAmountTable;
	static const QString k_CreateProductTable;
	static const QString k_CreateMeasureTable;
	static const QString k_CreateIngredientTable;
	static const QString k_CreateNutritionalDataTable;

    static QStringList readFile(const QString& fileName);
    static void fillTableFromFile(QSqlQuery& query, const QString& tableName, const QString fileName);
	static void fillAmountTable(QSqlQuery& query);
	static void fillProductTable(QSqlQuery& query);
	static void fillMeasureTable(QSqlQuery& query);
	static void fillNutritionTable(QtIngredientDao dao);
public:
    static void createSchemaOn(QSqlDatabase& db);
	static void fillInitialData(QSqlDatabase& db);
};

}
#endif
