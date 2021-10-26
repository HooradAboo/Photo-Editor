#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>
#include <algorithm>
#include <string.h>

struct roi {
    int x, y;
    int sx, sy;
};

struct interval {
    int a;
    int b;
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
    static void addGrey(image &src, image &tgt, char *input);
    static void binarize(image &src, image &tgt, char *input);
    static void scale(image &src, image &tgt, float ratio);
    static void adjustBrightness(image &src, image &tgt, char *input);
    static void smoothing(image &src, image &tgt, char *input);
    static void colorAdjustBrightness(image &src, image &tgt, char *input);
    static void colorBinarize(image &src, image &tgt, char *input);
    static void stretching(image &src, image &tgt, char *input);
    static void thresholdStretching(image &src, image &tgt, char *input);
    static void channelStretching(image &src, image &tgt, char *input);
    static void colorStretching(image &src, image &tgt, char *input);
    static void hueStretching(image &src, image &tgt, char *input); 
    static void hueSaturationStretching(image &src, image &tgt, char *input); 
    static void hueSaturationIntensityStretching(image &src, image &tgt, char *input);
    static void edgeDetection(image &src, image &tgt, char *input);
    static void thresholdEdgeDetection(image &src, image &tgt, char *input);
    static void colorEdgeDetection(image &src, image &tgt, char *input);
};

#endif

