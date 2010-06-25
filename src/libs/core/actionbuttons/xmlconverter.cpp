#include "xmlconverter.h"
#include <QXmlStreamWriter>
#include <QString>
#include <QTextCodec>
#include <core/db/recipe.h>

namespace Mamatkon {

QString XmlConverter::createXmlFrom(const Recipe& recipe) {
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

}
