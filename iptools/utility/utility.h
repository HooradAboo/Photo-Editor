#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>

struct roi    //rectangular region of intrest
{
    int x;
    int y;
    int sx;
    int sy;
    int value;
    int threshold;
};

class utility {
public:
    utility();
    virtual ~utility();
    static std::string intToString(int number);
    static int checkValue(int value);
    static void addGrey(image &src, image &tgt, vector<roi> rect);
    static void binarize(image &src, image &tgt, vector<roi> rect);
    static void scale(image &src, image &tgt, float ratio);
    static void adjustBrightness(image &src, image &tgt, int treshold, int value1, int value2, roi rect);
};

#endif

