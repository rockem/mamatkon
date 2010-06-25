#include "httpcommservice.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QString>
#include <core/actionbuttons/xmlconverter.h>

namespace Mamatkon {

void HttpCommService::sendRecipe(const Recipe& recipe) {
    QNetworkAccessManager *manager = new QNetworkAccessManager;
	QNetworkRequest request;
    QByteArray xml = XmlConverter::createXmlFrom(recipe).toUtf8();
	request.setUrl(QUrl("http://localhost:8080/maserver/addrecipe.service"));
	request.setRawHeader("Content-type", "text/xml; charset=UTF-8");
    request.setHeader(QNetworkRequest::ContentLengthHeader, xml.length());
    QNetworkReply *reply = manager->post(request, xml);
	QVariant status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    // dynamic_cast<ICommEventListener*>(eventListener())->sendingRecipeFinished(status.toInt());
}

}
