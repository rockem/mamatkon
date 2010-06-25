#ifndef HTTPCOMMSERVICE_H
#define HTTPCOMMSERVICE_H
#include <core/actionbuttons/commservice.h>
#include <QObject>

class QNetworkReply;

namespace Mamatkon {

class HttpCommService : public QObject, public ICommService {
    Q_OBJECT

public:
	void sendRecipe(const Recipe& recipe);
};

}

#endif // HTTPCOMMSERVICE_H
