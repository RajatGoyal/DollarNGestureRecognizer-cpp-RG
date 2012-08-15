#ifndef MULTISTROKEGESTURETEMPLATE_H
#define MULTISTROKEGESTURETEMPLATE_H
#include <string>

using namespace std;

namespace DollarRecognizer
{
class MultipleStrokeGestureTemplate
{
public:
        string name;
        MultiStrokeGesture paths;

        MultipleStrokeGestureTemplate(string name, MultiStrokeGesture paths)
        {
                this->name   = name;
                this->paths = paths;

        }
};

typedef vector<MultipleStrokeGestureTemplate> MultipleStrokeGestureTemplates;
}
#endif // MULTISTROKEGESTURETEMPLATE_H
