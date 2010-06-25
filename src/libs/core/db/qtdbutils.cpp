#include <core/db/qtdbutils.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QVariant>
#include <core/db/qtingredientdao.h>
#include <core/db/nutritionaldata.h>

namespace Mamatkon {

const QString QtDbUtils::k_CreateRecipeTable =
	"create table recipe ("
    "id INTEGER PRIMARY KEY, "
    "title TEXT, "
	"description TEXT, "
	"servings INTEGER, "
	"servings_scale INTEGER, "
	"source TEXT, "
	"tags TEXT, "
	"version_id INTEGER)";

const QString QtDbUtils::k_CreateAmountTable =
	"create table amount ("
	"id INTEGER PRIMARY KEY, "
	"name TEXT UNIQUE, "
	"value TEXT)";

const QString QtDbUtils::k_CreateProductTable =
	"create table product ("
	"id INTEGER PRIMARY KEY, "
	"name TEXT UNIQUE)";

const QString QtDbUtils::k_CreateMeasureTable =
	"create table measure ("
	"id INTEGER PRIMARY KEY, "
	"name TEXT UNIQUE, "
	"ratioToGram FLOAT)";

const QString QtDbUtils::k_CreateIngredientTable  =
	"create table ingredient ("
	"id INTEGER PRIMARY KEY, "
	"recipe_id INTEGER, "
	"ingredient_id INTEGER, "
	"product TEXT, "
	"amount TEXT, "
	"measure TEXT, "
	"method TEXT, "
	"alt_id INTEGER)";

const QString QtDbUtils::k_CreateNutritionalDataTable =	
	"create table nutritional_data ("
	"id INTEGER PRIMARY KEY, "
	"product TEXT UNIQUE, "
	"unit_weight INTEGER, " 
	"calories FLOAT, "
	"protein FLOAT, "
	"carbohydrate FLOAT, " 
	"fat FLOAT, "
	"iron FLOAT, "
	"calcium FLOAT, " 
	"vitaminc FLOAT, "
	"potassium FLOAT, "
	"fiber FLOAT, "
	"vitamina FLOAT, " 
	"cholesterol FLOAT, " 
	"saturated_fat FLOAT, "
	"b12 FLOAT, "
	"sodium FLOAT, " 
	"pholic_acid FLOAT, " 
	"omega3 FLOAT)";

void QtDbUtils::createSchemaOn(QSqlDatabase& db) {
    QSqlQuery query(db);
    query.exec(k_CreateRecipeTable);
	query.exec(k_CreateAmountTable);
	query.exec(k_CreateProductTable);
	query.exec(k_CreateMeasureTable);
	query.exec(k_CreateIngredientTable);
	query.exec(k_CreateNutritionalDataTable);


    db.commit();
}

QStringList QtDbUtils::readFile(const QString& fileName) {
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return QStringList();

	QStringList fileRows;
	QTextStream in(&file);
	in.setCodec("UTF-8");
	while (!in.atEnd()) {
		QString line = in.readLine();
		if(line != "") {
			fileRows.append(line);
		}
	}

	return fileRows;
}

void QtDbUtils::fillAmountTable( QSqlQuery& query ) {
	QStringList list = readFile("../mamatkon-amounts");
	query.prepare("insert into amount (name, value) values(:name, :value)");
	for (int i = 0; i < list.size(); ++i) {
		if(!list[i].isEmpty()) {
			QStringList row = list[i].split(",");
			query.bindValue(":name", QVariant::fromValue(row[0]));
			query.bindValue(":value", QVariant::fromValue(row[1]));
			query.exec();
		}
	}
}

void QtDbUtils::fillTableFromFile(QSqlQuery& query, const QString& tableName, const QString fileName) {
	QStringList list = readFile(fileName);
	query.prepare("insert into " + tableName + " (name) values(:name)");
	for (int i = 0; i < list.size(); ++i) {
		if(!list[i].isEmpty()) {
			query.bindValue(":name", QVariant::fromValue(list[i]));
			query.exec();
		}
	}
}

void QtDbUtils::fillProductTable( QSqlQuery& query ) {
	fillTableFromFile(query, "product", "../mamatkon-products");
}

void QtDbUtils::fillMeasureTable( QSqlQuery& query ) {
	QStringList list = readFile("../mamatkon-measures");
	query.prepare("insert into measure (name, ratioToGram) values(:name, :ratioToGram)");
	for (int i = 0; i < list.size(); ++i) {
		if(!list[i].isEmpty()) {
			QStringList row = list[i].split(",");
			query.bindValue(":name", QVariant::fromValue(row[0]));
			query.bindValue(":ratioToGram", QVariant::fromValue(row[1]));
			query.exec();
		}
	}
}

void QtDbUtils::fillInitialData(QSqlDatabase& db) {
	QSqlQuery query(db);
	fillAmountTable(query);
	fillProductTable(query);
	fillMeasureTable(query);
	fillNutritionTable(QtIngredientDao(db));
}

void QtDbUtils::fillNutritionTable(QtIngredientDao dao) {
	QStringList list = readFile("../mamatkon-nutrition");
	for (int i = 1; i < list.size(); ++i) {
		if(!list[i].isEmpty()) {
			NutritionalData data;
			QStringList row = list[i].split(",");
			if(row.size() > 1) data._UnitWeight = row[1].toInt();
			if(row.size() > 2) data._NutritionalValues[NutritionalData::Calories    ] = row[2].toDouble();
			if(row.size() > 3) data._NutritionalValues[NutritionalData::Protein     ] = row[3].toDouble();
			if(row.size() > 4) data._NutritionalValues[NutritionalData::Carbohydrate] = row[4].toDouble();
			if(row.size() > 5) data._NutritionalValues[NutritionalData::Fat         ] = row[5].toDouble();
			if(row.size() > 6) data._NutritionalValues[NutritionalData::Iron        ] = row[6].toDouble();
			if(row.size() > 7) data._NutritionalValues[NutritionalData::Calcium     ] = row[7].toDouble();
			if(row.size() > 8) data._NutritionalValues[NutritionalData::VitaminC    ] = row[8].toDouble();
			if(row.size() > 9) data._NutritionalValues[NutritionalData::Potassium   ] = row[9].toDouble();
			if(row.size() > 10) data._NutritionalValues[NutritionalData::Fiber       ] = row[10].toDouble();
			if(row.size() > 11) data._NutritionalValues[NutritionalData::VitaminA    ] = row[11].toDouble();
			if(row.size() > 12) data._NutritionalValues[NutritionalData::Cholesterol ] = row[12].toDouble();
			if(row.size() > 13) data._NutritionalValues[NutritionalData::SaturatedFat] = row[13].toDouble();
			if(row.size() > 14) data._NutritionalValues[NutritionalData::B12         ] = row[14].toDouble();
			if(row.size() > 15) data._NutritionalValues[NutritionalData::Sodium      ] = row[15].toDouble();
			if(row.size() > 16) data._NutritionalValues[NutritionalData::PholicAcid  ] = row[16].toDouble();
			if(row.size() > 17) data._NutritionalValues[NutritionalData::Omega3      ] = row[17].toDouble();
			dao.insertNutritionalData(row[0], data);
		}
	}
}
}
