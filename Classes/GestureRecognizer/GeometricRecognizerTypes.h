#ifndef _GeometricRecognizerTypesIncluded_
#define _GeometricRecognizerTypesIncluded_

#include <limits>
#include <math.h>
#include <list>
#include <vector>

using namespace std;

namespace FingerRecognizer
{
    enum E_INPUT_KIND{
        E_INPUT_KIND_ONE_CLICK = -2,
        E_INPUT_KIND_DOUBLE_CLICK = -1,
        
        E_INPUT_KIND_EIGHT = 0,
        E_INPUT_KIND_VERTICAL = 1,
        E_INPUT_KIND_V = 2,
        E_INPUT_KIND_V_VERTICAL_FLIP = 3,
        E_INPUT_KIND_HORIZONTAL = 4,
        E_INPUT_KIND_Z = 5,
        E_INPUT_KIND_U = 6,
        E_INPUT_KIND_N = 7,
        E_INPUT_KIND_LIKE_EIGHT = 8,
        E_INPUT_KIND_LIKE_SHIT = 9,
        E_INPUT_KIND_LIKE_SHIT_2 = 10,
        E_INPUT_KIND_LIKE_SHEEP = 11,
        E_INPUT_KIND_LIKE_FISH = 12,
        E_INPUT_KIND_LIKE_WAVE = 13,
        
        E_INPUT_KIND_ICE_GUN = 14,
        E_INPUT_KIND_TURRET = 15,
        E_INPUT_KIND_BLESSING = 16,
        E_INPUT_KIND_BOW_SKILL = 17,
        E_INPUT_KIND_FLASH = 18,
        E_INPUT_KIND_SLOW_DOWN = 27,
        E_INPUT_KIND_DEFENSE = 28,
        
        E_INPUT_KIND_UNKNOW = 37,
    };
    class Point2D
    {
    public:
        //--- Wobbrock used doubles for these, not ints
        //int x, y;
        double x, y;
        Point2D() 
        {
            this->x=0; 
            this->y=0;
        }
        Point2D(double x, double y)
        {
            this->x = x;
            this->y = y;
        }
    };

    typedef vector<Point2D>  Path2D;
    typedef Path2D::iterator Path2DIterator;

    class Rectangle
    {
    public:
        double x, y, width, height;
        Rectangle(double x, double y, double width, double height)
        {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
        }
    };

    class RecognitionResult
    {
    public:
        E_INPUT_KIND name;
        double score;
        RecognitionResult(E_INPUT_KIND name, double score)
        {
            this->name = name;
            this->score = score;
        }
    };
}
#endif