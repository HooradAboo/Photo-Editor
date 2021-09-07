#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

std::string utility::intToString(int number) {
    std::stringstream ss;//create a stringstream
    ss << number;//add number to the stream
    return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value) {
    if (value > MAXRGB)
        return MAXRGB;
    if (value < MINRGB)
        return MINRGB;
    return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, vector<roi> rect) {
    // Make a copy of origin image to target image
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns()); 
    for (int i = 0; i < src.getNumberOfRows(); i++) {
        for (int j = 0; j < src.getNumberOfColumns(); j++) {
            tgt.setPixel(i, j, src.getPixel(i, j));
        }
    }

    // Print vector
    for (int i = 0; i < rect.size(); i++) {
        cout << "Value: " << rect[i].value << endl;
        cout << "(X,Y): (" << rect[i].x << "," << rect[i].y << ")" << endl;
        cout << "(SX,SY): (" << rect[i].sx << "," << rect[i].sy << ")" << endl;
    }
    
    // Make changes to region of intrest
    for (int r = 0; r < rect.size(); r++) {
        for (int i = rect[r].y; i < rect[r].sy; i++) {
            for (int j = rect[r].x; j < rect[r].sx; j++) {
                tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + rect[r].value));
            }
        }
    }
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int threshold, roi rect) {
    // Make a copy of origin image to target image
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    for (int i = 0; i < src.getNumberOfRows(); i++) {
        for (int j = 0; j < src.getNumberOfColumns(); j++) {
            tgt.setPixel(i, j, src.getPixel(i, j));
        }
    }

    // Make changes to region of intrest
    for (int i = rect.y; i < rect.sy; i++) {
        for (int j = rect.x; j < rect.sx; j++) {
            if (src.getPixel(i, j) < threshold){
                tgt.setPixel(i, j, MINRGB);
            } else {
                tgt.setPixel(i, j, MAXRGB);
            }
        }
    }
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio) {
    int rows = (int) ((float) src.getNumberOfRows() * ratio);
    int cols = (int) ((float) src.getNumberOfColumns() * ratio);
    tgt.resize(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            /* Map the pixel of new image back to original image */
            int i2 = (int) floor((float) i / ratio);
            int j2 = (int) floor((float) j / ratio);
            if (ratio == 2) {
                /* Directly copy the value */
                tgt.setPixel(i, j, checkValue(src.getPixel(i2, j2)));
            }

            if (ratio == 0.5) {
                /* Average the values of four pixels */
                int value = src.getPixel(i2, j2) + src.getPixel(i2, j2 + 1) + src.getPixel(i2 + 1, j2) +
                            src.getPixel(i2 + 1, j2 + 1);
                tgt.setPixel(i, j, checkValue(value / 4));
            }
        }
    }
}

/*-----------------------------------------------------------------------**/
void utility::adjustBrightness(image &src, image &tgt, int treshold, int value1, int value2, roi rect) {
    // Make a copy of origin image to target image
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    for (int i = 0; i < src.getNumberOfRows(); i++) {
        for (int j = 0; j < src.getNumberOfColumns(); j++) {
            tgt.setPixel(i, j, src.getPixel(i, j));
        }
    }

    // Make changes to region of intrest
    for (int i = rect.y; i < rect.sy; i++) {
        for (int j = rect.x; j < rect.sx; j++) {
            if (src.getPixel(i, j) > treshold) {
                tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + value1));
            } else if (src.getPixel(i, j) < treshold) {
                tgt.setPixel(i, j, checkValue(src.getPixel(i, j) - value2));
            }
        }
    }
}