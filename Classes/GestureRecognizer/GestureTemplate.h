#ifndef _GestureTemplateIncluded_
#define _GestureTemplateIncluded_

using namespace std;

namespace FingerRecognizer
{
class GestureTemplate
{
public:
    E_INPUT_KIND name;
    Path2D points;
    
    GestureTemplate(E_INPUT_KIND name, Path2D points)
    {
        this->name   = name;
        this->points = points;
    }
};

typedef vector<GestureTemplate> GestureTemplates;
}

#endif