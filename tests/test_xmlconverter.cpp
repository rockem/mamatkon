#include <gmock/gmock.h>
#include <core/actionbuttons/xmlconverter.h>
#include <core/db/recipe.h>
#include <QtDebug>

namespace MaTest {
namespace {

using Mamatkon::Recipe;
using Mamatkon::XmlConverter;

TEST(XmlConverterTest, TestShouldRetrieveXmlRepresentationOfRecipe) {
	QString title("titl");
	QString instructions("do the and that");
	QString source("mom");
	QString servings("4");

	QString s;
	s.append("<?xml version=\"1.0\"?>").
		append("<recipe>").
		append("<title>").append(title).append("</title>").
		append("<instructions>").append(instructions).append("</instructions>").
		append("<source>").append(source).append("</source>").
		append("<servings>").append(servings).append("</servings>").
		append("<ingredients>").append("</ingredients>").
		append("</recipe>");

	Recipe r(3, title);
	r.Title = title;
	r.Instruction = instructions;
	r.Source = source;
	r.Servings = servings.toInt();

	EXPECT_TRUE(s == XmlConverter::createXmlFrom(r));
}

}
}
