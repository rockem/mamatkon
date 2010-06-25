#ifndef BASEDAO_H
#define BASEDAO_H
#include <QString>
#include <QSqlDatabase>
#include <vector>

namespace Mamatkon { 

	class QtBaseDao {
		QSqlDatabase& m_db;
	public:
		QtBaseDao(QSqlDatabase & db) : m_db(db) {}
		virtual ~QtBaseDao() {}

		QSqlDatabase& database() const { return m_db; }
		std::vector<long> findRecipeIdsBy(QString query, std::vector<QString> words) const;
	};

}
#endif // BASEDAO_H