#ifndef _GestureTemplateIncluded_
#define _GestureTemplateIncluded_

#include <string>

using namespace std;

namespace DollarRecognizer
{
class GestureTemplate
{
public:
	string name;
	Path2D points;
        Point2D startv;
        vector<double> Vector;
        GestureTemplate(string name, Path2D points,Point2D startv,vector<double> Vector)
	{
		this->name   = name;
		this->points = points;
            this->startv = startv;
            this->Vector =Vector;
	}

        GestureTemplate(string name, Path2D points)
        {
                this->name   = name;
                this->points = points;
        }
};

typedef vector<GestureTemplate>  GestureTemplates;
}

#endif
