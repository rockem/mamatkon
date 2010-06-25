#include "actionbuttonscontrol.h"
#include <core/ui/uifactory.h>
#include <core/actionbuttons/actionbuttonsui.h>
#include <core/db/recipedao.h>
#include <core/recipeevent.h>
#include <boost/shared_ptr.hpp>
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QXmlStreamWriter>
#include <boost/pointer_cast.hpp>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace Mamatkon {

using ::std::map;
using ::boost::shared_ptr;
using ::boost::static_pointer_cast;

ActionButtonsControl::ActionButtonsControl( EventManager& em ) : ControlBase(em) {
    eventManager().addEventListener(Event::SelectedRecipeChanged, METHOD_MEMEBER(ActionButtonsControl::onRecipeSelectionChanged));
}





void ActionButtonsControl::onRecipeSelectionChanged(::boost::shared_ptr<Event> e) {
    m_currentRecipeId = static_pointer_cast<RecipeEvent>(e)->recipeId();
}



QString createXmlFrom(const Recipe& recipe) {
    QString xml;
    QXmlStreamWriter stream(&xml);
    stream.writeStartDocument();
    stream.writeStartElement("recipe");
    stream.writeTextElement("title", recipe.Title);
    stream.writeTextElement("instructions", recipe.Instruction);
    stream.writeTextElement("source", recipe.Source);
    QString s;
    s.setNum(recipe.Servings);
    stream.writeTextElement("servings", s);
    stream.writeTextElement("ingredients", "");
    // stream.writeEndElement(); // ingredients
    stream.writeEndElement(); // recipe
    stream.writeEndDocument();
    xml.chop(1); // remove last \n character
    return xml;
}

void sendRecipe(const Recipe& recipe) {
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkRequest request;
    QByteArray xml = createXmlFrom(recipe).toUtf8();
    request.setUrl(QUrl("http://localhost:8080/maserver/addrecipe.service"));
    request.setRawHeader("Content-type", "text/xml; charset=UTF-8");
    request.setHeader(QNetworkRequest::ContentLengthHeader, xml.length());
    QNetworkReply *reply = manager->post(request, xml);
    QVariant status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    // dynamic_cast<ICommEventListener*>(eventListener())->sendingRecipeFinished(status.toInt());
}

void ActionButtonsControl::dispatchClick(int buttonNumber) const {
    if(buttonNumber == 1) {
        long id = recipeDao()->createNewRecipe();
        eventManager().raiseEvent(shared_ptr<RecipeEvent>(new RecipeEvent(Event::NewRecipeCreated, id)));
    }
    if(buttonNumber == 4) {
        Recipe r = recipeDao()->getRecipeById(m_currentRecipeId);
        sendRecipe(r);
    }
}

}
