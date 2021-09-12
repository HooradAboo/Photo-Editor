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
void utility::addGrey(image &src, image &tgt, vector<v_roi> rect) {
    // bool flag = checkRoI(rect);
    // cout << flag << endl;

    // Make a copy of origin image to target image
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns()); 
    for (int i = 0; i < src.getNumberOfRows(); i++) {
        for (int j = 0; j < src.getNumberOfColumns(); j++) {
            tgt.setPixel(i, j, src.getPixel(i, j));
        }
    }

    // Print vector
    for (int i = 0; i < rect.size(); i++) {
        cout << "/*------------------------*/" << endl;
        cout << "Add Gray" << endl;
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
void utility::binarize(image &src, image &tgt, vector<t_roi> rect) {
    // Make a copy of origin image to target image
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    for (int i = 0; i < src.getNumberOfRows(); i++) {
        for (int j = 0; j < src.getNumberOfColumns(); j++) {
            tgt.setPixel(i, j, src.getPixel(i, j));
        }
    }

    // Print vector
    for (int i = 0; i < rect.size(); i++) {
        cout << "/*------------------------*/" << endl;
        cout << "Binarize" << endl;
        cout << "Threshold: " << rect[i].threshold << endl;
        cout << "(X,Y): (" << rect[i].x << "," << rect[i].y << ")" << endl;
        cout << "(SX,SY): (" << rect[i].sx << "," << rect[i].sy << ")" << endl;
    }

    // Make changes to region of intrest
    for (int r = 0; r < rect.size(); r++) {
        for (int i = rect[r].y; i < rect[r].sy; i++) {
            for (int j = rect[r].x; j < rect[r].sx; j++) {
                if (src.getPixel(i, j) < rect[r].threshold){
                    tgt.setPixel(i, j, MINRGB);
                } else {
                    tgt.setPixel(i, j, MAXRGB);
                }
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
void utility::adjustBrightness(image &src, image &tgt, vector<vtv_roi> rect) {
    // Make a copy of origin image to target image
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    for (int i = 0; i < src.getNumberOfRows(); i++) {
        for (int j = 0; j < src.getNumberOfColumns(); j++) {
            tgt.setPixel(i, j, src.getPixel(i, j));
        }
    }

    // Print vector
    for (int i = 0; i < rect.size(); i++) {
        cout << "/*------------------------*/" << endl;
        cout << "Adjust Brightness" << endl;
        cout << "Threshold: " << rect[i].threshold << endl;
        cout << "Value1, Value2: " << rect[i].value1 << ", " << rect[i].value1 << endl;
        cout << "(X,Y): (" << rect[i].x << "," << rect[i].y << ")" << endl;
        cout << "(SX,SY): (" << rect[i].sx << "," << rect[i].sy << ")" << endl;
    }

    // Make changes to region of intrest
    for (int r = 0; r < rect.size(); r++) {
        for (int i = rect[r].y; i < rect[r].sy; i++) {
            for (int j = rect[r].x; j < rect[r].sx; j++) {
                if (src.getPixel(i, j) > rect[r].threshold) {
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + rect[r].value1));
                } else if (src.getPixel(i, j) < rect[r].threshold) {
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j) - rect[r].value2));
                }
            }
        }
    }
}

/*-----------------------------------------------------------------------**/
void utility::smoothing(image &src, image &tgt, vector<w_roi> rect) {
    // Make a copy of origin image to target image
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    for (int i = 0; i < src.getNumberOfRows(); i++) {
        for (int j = 0; j < src.getNumberOfColumns(); j++) {
            tgt.setPixel(i, j, src.getPixel(i, j));
        }
    }

    // Print vector
    for (int i = 0; i < rect.size(); i++) {
        cout << "/*------------------------*/" << endl;
        cout << "Smoothing" << endl;
        cout << "Window: " << rect[i].window << endl;
        cout << "(X,Y): (" << rect[i].x << "," << rect[i].y << ")" << endl;
        cout << "(SX,SY): (" << rect[i].sx << "," << rect[i].sy << ")" << endl;
    }


    // Make changes to region of intrest
    for (int r = 0; r < rect.size(); r++) {
        for (int i = rect[r].y; i < rect[r].sy; i++) {
            for (int j = rect[r].x; j < rect[r].sx; j++) {
                int sum = 0;
                for (int wr = 0; wr < rect[r].window; wr++) {
                    for (int wc = 0; wc < rect[r].window; wc++) {
                        sum += tgt.getPixel(i + wr, j + wc);
                    }
                }
                int avg = sum / (rect[r].window * rect[r].window);
                tgt.setPixel(i + rect[r].window/2, j + rect[r].window/2, checkValue(avg));
            }
        }
    }
}