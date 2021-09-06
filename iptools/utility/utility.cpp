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
void utility::addGrey(image &src, image &tgt, int value, roi rect) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    cout << value << endl;
    cout << rect.x << rect.sx << endl;
    cout << rect.y << rect.sy << endl;
    // for (int r = 0; r < rect.size(); r++)
    for (int i = 0; i < src.getNumberOfRows(); i++) {
        for (int j = 0; j < src.getNumberOfColumns(); j++) {
            tgt.setPixel(i, j, RED, src.getPixel(i, j, RED));
        }
    }
    // for (int i = 0; i < src.getNumberOfRows(); i++) {
    //     for (int j = 0; j < src.getNumberOfColumns(); j++) {
    //         tgt.setPixel(i, j, GREEN, src.getPixel(i, j, GREEN));
    //     }
    // }
    // for (int i = 0; i < src.getNumberOfRows(); i++) {
    //     for (int j = 0; j < src.getNumberOfColumns(); j++) {
    //         tgt.setPixel(i, j, BLUE, src.getPixel(i, j, BLUE));
    //     }
    // }
    for (int i = rect.y; i < rect.sy; i++) {
        for (int j = rect.x; j < rect.sx; j++) {
            tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + value));
        }
    }
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int threshold) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    for (int i = 0; i < src.getNumberOfRows(); i++) {
        for (int j = 0; j < src.getNumberOfColumns(); j++) {
            if (src.getPixel(i, j) < threshold)
                tgt.setPixel(i, j, MINRGB);
            else
                tgt.setPixel(i, j, MAXRGB);
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
void utility::adjustBrightness(image &src, image &tgt, int treshold, int value1, int value2) {
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    for (int i = 0; i < src.getNumberOfRows(); i++) {
        for (int j = 0; j < src.getNumberOfColumns(); j++) {
            if (src.getPixel(i, j) > treshold) {
                // cout << "inja" << endl;
                tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + value1));
            } else if (src.getPixel(i, j) < treshold) {
                // cout << "inja2" << endl;
                tgt.setPixel(i, j, checkValue(src.getPixel(i, j) - value2));
            }
        }
    }
}