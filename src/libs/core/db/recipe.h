#ifndef _RECIPE_H
#define _RECIPE_H
#include <ostream>
#include <QString>

namespace Mamatkon {

struct Recipe {

	long Id;
	QString Title;
    QString Instruction;
    int Servings;
	int ServingsScale;
    QString Source;
	QString Tags;


	Recipe() : 
		Id(0),
		Servings(1), 
		ServingsScale(1) {}

    Recipe(long id, const QString& title) :
            Id(id),
            Title(title),
            Servings(1),
			ServingsScale(1) {}
	
		Recipe(
			long id, 
			const QString& title,
			const QString& instructions,
			int servings,
			int servingsScale,
			QString source,
			QString tags) :
		Id(id),
			Title(title),
			Instruction(instructions),
			Servings(servings),
			ServingsScale(servingsScale),
			Source(source),
			Tags(tags) {}


    bool operator ==(const Recipe& other) const {
		return 
			Id == other.Id &&
			Title == other.Title && 
			Instruction == other.Instruction && 
			Servings == other.Servings &&
			ServingsScale == other.ServingsScale &&
			Source == other.Source && 
			Tags == other.Tags;
			
	}

	friend ::std::ostream& operator<<(::std::ostream& s, const Recipe& r) {
		s<<"title:"<<r.Title.toStdString();
		return s;
	}

};

}
#endif
