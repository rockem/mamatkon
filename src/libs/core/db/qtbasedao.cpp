#include "qtbasedao.h"
#include <QSqlQuery>
#include <set>
#include <QVariant>

using namespace Mamatkon;
using std::vector;
using std::set;

std::vector<long> Mamatkon::QtBaseDao::findRecipeIdsBy(QString query, std::vector<QString> words) const {
	QSqlQuery q(m_db);
	set<long> idsSet;
	vector<QString>::const_iterator it = words.begin();
	for(; it != words.end(); ++it) {
		q.exec(QString(query).arg(*it));
		q.exec();
		while(q.next()) {
			idsSet.insert(q.value(0).toLongLong());
		}
	}
	vector<long> ids;
	copy(idsSet.begin(), idsSet.end(), inserter(ids, ids.end()));
	return ids;
}