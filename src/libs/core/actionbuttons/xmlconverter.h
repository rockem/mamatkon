#ifndef XMLCONVERTER_H
#define XMLCONVERTER_H
#include <QString>

namespace Mamatkon {

struct Recipe;

class XmlConverter {
public:
	static QString createXmlFrom(const Recipe& recipe);

};

}


#endif // XMLCONVERTER_H