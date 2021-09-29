#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>
#include <algorithm>

struct roi {
    int x, y;
    int sx, sy;
};

struct range {
    int a;
    int b;
};

// Rectangular region of intrest - addGray
struct v_roi {
    int x, y;
    int sx, sy;
    int value;
};

// Rectangular region of intrest - binarize
struct t_roi {
    int x, y;
    int sx, sy;
    int threshold;
};

// Rectangular region of intrest - addjustBrightness
struct vtv_roi {
    int x, y;
    int sx, sy;
    int value1, value2;
    int threshold;
};

// Rectangular region of intrest - smoothing
struct w_roi {
    int x, y;
    int sx, sy;
    int window;
};

// Rectangular region of intrest - colorAddjustBrightness
struct cm_roi {
    int x, y;
    int sx, sy;
    int more_c;
};

// Rectangular region of intrest - colorBinarization
struct cb_roi {
    int x, y;
    int sx, sy;
    int tc;
    int cr, cg, cb;
};

class utility {
public:
    utility();
    virtual ~utility();
    static std::string intToString(int number);
    static int checkValue(int value);
    static void addGrey(image &src, image &tgt, vector<v_roi> rect);
    static void binarize(image &src, image &tgt, vector<t_roi> rect);
    static void scale(image &src, image &tgt, float ratio);
    static void adjustBrightness(image &src, image &tgt, vector<vtv_roi> rect);
    static void smoothing(image &src, image &tgt, vector<w_roi> rect);
    static void colorAdjustBrightness(image &src, image &tgt, vector<cm_roi> rect);
    static void colorBinarize(image &src, image &tgt, vector<cb_roi> rect);
    static void histogramStretching(image &src, roi rect);
};

#endif

