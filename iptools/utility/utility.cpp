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

template <typename RoI>
bool checkRoI (vector<RoI> vroi) {
    for (int i = 0; i < vroi.size(); i++){
        // If (X,Y) is equals or greater than (SX,Sy), print error message.
        if ((vroi[i].x >= vroi[i].sx) || (vroi[i].y >= vroi[i].sy)) {
            switch (i+1) {
            case 1:
                cout << "Error 101: 1st ROI cooardinates are not correct." << endl;
                return false;
            case 2:
                cout << "Error 101: 2ed ROI cooardinates are not correct." << endl;
                return false;
            case 3:
                cout << "Error 101: 3rd ROI cooardinates are not correct." << endl;
                return false;
            default:
                cout << "Error 101: " << i+1 << "th  ROI cooardinates are not correct." << endl;
                return false;
            }
        }

        // If any two RoI have overlaped, print error message.
        for (int j = i + 1; j < vroi.size(); j++){

            if (vroi[i].x <= vroi[j].x && vroi[j].x <= vroi[i].sx) {
                if (vroi[i].y <= vroi[j].y && vroi[j].y <= vroi[i].sy) {
                    cout << "Error 201: RoI " << i+1 << " and " << j+1 << " have overlapped." << endl;
                    return false;
                }
            }
            if (vroi[i].x <= vroi[j].sx && vroi[j].sx <= vroi[i].sx) {
                if (vroi[i].y <= vroi[j].y && vroi[j].y <= vroi[i].sy) {
                    cout << "Error 202: RoI " << i+1 << " and " << j+1 << " have overlapped." << endl;
                    return false;
                }
            }
            if (vroi[i].x <= vroi[j].x && vroi[j].x <= vroi[i].sx) {
                if (vroi[i].y <= vroi[j].sy && vroi[j].sy <= vroi[i].sy) {
                    cout << "Error 203: RoI " << i+1 << " and " << j+1 << " have overlapped." << endl;
                    return false;
                }
            }
            if (vroi[i].x <= vroi[j].sx && vroi[j].sx <= vroi[i].sx) {
                if (vroi[i].y <= vroi[j].sy && vroi[j].sy <= vroi[i].sy) {
                    cout << "Error 204: RoI " << i+1 << " and " << j+1 << " have overlapped." << endl;
                    return false;
                }
            }         
        }
    }
    return true;
}


/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, vector<v_roi> rect) {
    if (checkRoI (rect)) {
        // Make a copy of origin image to target image
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns()); 
        for (int i = 0; i < src.getNumberOfRows(); i++) {
            for (int j = 0; j < src.getNumberOfColumns(); j++) {
                tgt.setPixel(i, j, src.getPixel(i, j));
            }
        }

        // // Print vector
        // for (int i = 0; i < rect.size(); i++) {
        //     cout << "/*------------------------*/" << endl;
        //     cout << "Add Gray" << endl;
        //     cout << "Value: " << rect[i].value << endl;
        //     cout << "(X,Y): (" << rect[i].x << "," << rect[i].y << ")" << endl;
        //     cout << "(SX,SY): (" << rect[i].sx << "," << rect[i].sy << ")" << endl;
        // }
        
        // Make changes to region of intrest
        for (int r = 0; r < rect.size(); r++) {
            for (int i = rect[r].y; i < rect[r].sy; i++) {
                for (int j = rect[r].x; j < rect[r].sx; j++) {
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + rect[r].value));
                }
            }
        }
    }
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, vector<t_roi> rect) {
    if (checkRoI (rect)) {
        // Make a copy of origin image to target image
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        for (int i = 0; i < src.getNumberOfRows(); i++) {
            for (int j = 0; j < src.getNumberOfColumns(); j++) {
                tgt.setPixel(i, j, src.getPixel(i, j));
            }
        }

        // // Print vector
        // for (int i = 0; i < rect.size(); i++) {
        //     cout << "/*------------------------*/" << endl;
        //     cout << "Binarize" << endl;
        //     cout << "Threshold: " << rect[i].threshold << endl;
        //     cout << "(X,Y): (" << rect[i].x << "," << rect[i].y << ")" << endl;
        //     cout << "(SX,SY): (" << rect[i].sx << "," << rect[i].sy << ")" << endl;
        // }

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
    if (checkRoI (rect)) {
        // Make a copy of origin image to target image
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        for (int i = 0; i < src.getNumberOfRows(); i++) {
            for (int j = 0; j < src.getNumberOfColumns(); j++) {
                tgt.setPixel(i, j, src.getPixel(i, j));
            }
        }

        // // Print vector
        // for (int i = 0; i < rect.size(); i++) {
        //     cout << "/*------------------------*/" << endl;
        //     cout << "Adjust Brightness" << endl;
        //     cout << "Threshold: " << rect[i].threshold << endl;
        //     cout << "Value1, Value2: " << rect[i].value1 << ", " << rect[i].value2 << endl;
        //     cout << "(X,Y): (" << rect[i].x << "," << rect[i].y << ")" << endl;
        //     cout << "(SX,SY): (" << rect[i].sx << "," << rect[i].sy << ")" << endl;
        // }

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
}

/*-----------------------------------------------------------------------**/
void utility::smoothing(image &src, image &tgt, vector<w_roi> rect) {
    if (checkRoI (rect)) {
        // Make a copy of origin image to target image
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        for (int i = 0; i < src.getNumberOfRows(); i++) {
            for (int j = 0; j < src.getNumberOfColumns(); j++) {
                tgt.setPixel(i, j, src.getPixel(i, j));
            }
        }

        // // Print vector
        // for (int i = 0; i < rect.size(); i++) {
        //     cout << "/*------------------------*/" << endl;
        //     cout << "Smoothing" << endl;
        //     cout << "Window: " << rect[i].window << endl;
        //     cout << "(X,Y): (" << rect[i].x << "," << rect[i].y << ")" << endl;
        //     cout << "(SX,SY): (" << rect[i].sx << "," << rect[i].sy << ")" << endl;
        // }


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
                    int avg = sum / pow(rect[r].window, 2);
                    tgt.setPixel(i + rect[r].window/2, j + rect[r].window/2, checkValue(avg));
                }
            }
        }
    }
}

/*-----------------------------------------------------------------------**/
void utility::colorAdjustBrightness(image &src, image &tgt, vector<cm_roi> rect) {
    if (checkRoI (rect)) {
        // Make a copy of origin image to target image
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        for (int i = 0; i < src.getNumberOfRows(); i++) {
            for (int j = 0; j < src.getNumberOfColumns(); j++) {
                tgt.setPixel(i, j, RED, src.getPixel(i, j, RED));
                tgt.setPixel(i, j, GREEN, src.getPixel(i, j, GREEN));
                tgt.setPixel(i, j, BLUE, src.getPixel(i, j, BLUE));
            }
        }

        // // Print vector
        // for (int i = 0; i < rect.size(); i++) {
        //     cout << "/*------------------------*/" << endl;
        //     cout << "Color Brightness Modifying" << endl;
        //     cout << "More-C: " << rect[i].more_c << endl;
        //     cout << "(X,Y): (" << rect[i].x << "," << rect[i].y << ")" << endl;
        //     cout << "(SX,SY): (" << rect[i].sx << "," << rect[i].sy << ")" << endl;
        // }


        // Make changes to region of intrest
        for (int r = 0; r < rect.size(); r++) {
            for (int i = rect[r].y; i < rect[r].sy; i++) {
                for (int j = rect[r].x; j < rect[r].sx; j++) {
                    tgt.setPixel(i, j, RED, checkValue(src.getPixel(i, j, RED) * rect[r].more_c));
                    tgt.setPixel(i, j, GREEN, checkValue(src.getPixel(i, j, GREEN) + rect[r].more_c));
                    tgt.setPixel(i, j, BLUE, checkValue(src.getPixel(i, j, BLUE) + rect[r].more_c));
                }
            }
        }
    }
}

/*-----------------------------------------------------------------------**/
void utility::colorBinarize(image &src, image &tgt, vector<cb_roi> rect) {
    if (checkRoI (rect)) {
        // Make a copy of origin image to target image
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        for (int i = 0; i < src.getNumberOfRows(); i++) {
            for (int j = 0; j < src.getNumberOfColumns(); j++) {
                tgt.setPixel(i, j, RED, src.getPixel(i, j, RED));
                tgt.setPixel(i, j, GREEN, src.getPixel(i, j, GREEN));
                tgt.setPixel(i, j, BLUE, src.getPixel(i, j, BLUE));
            }
        }

        // // Print vector
        // for (int i = 0; i < rect.size(); i++) {
        //     cout << "/*------------------------*/" << endl;
        //     cout << "Color Binarize" << endl;
        //     cout << "(CR,CG,CB): (" << rect[i].cr << "," << rect[i].cg << "," << rect[i].cb << ")" << endl;
        //     cout << "(X,Y): (" << rect[i].x << "," << rect[i].y << ")" << endl;
        //     cout << "(SX,SY): (" << rect[i].sx << "," << rect[i].sy << ")" << endl;
        // }

        int tc_original;
        // Make changes to region of intrest
        for (int r = 0; r < rect.size(); r++) {
            for (int i = rect[r].y; i < rect[r].sy; i++) {
                for (int j = rect[r].x; j < rect[r].sx; j++) {
                    tc_original = sqrt (pow(rect[r].cr-src.getPixel(i, j, RED),2) + pow(rect[r].cg-src.getPixel(i, j, GREEN),2) + pow(rect[r].cb-src.getPixel(i, j, BLUE),2));
                    if (tc_original < rect[r].tc){
                        tgt.setPixel(i, j, RED, MAXRGB);
                        tgt.setPixel(i, j, GREEN, MINRGB);
                        tgt.setPixel(i, j, BLUE, MINRGB);
                    } else if (rect[r].tc <= tc_original && tc_original <= 2*rect[r].tc) {
                        tgt.setPixel(i, j, RED, MINRGB);
                        tgt.setPixel(i, j, GREEN, MAXRGB);
                        tgt.setPixel(i, j, BLUE, MINRGB);
                    } else {
                        tgt.setPixel(i, j, RED, MAXRGB);
                        tgt.setPixel(i, j, GREEN, MAXRGB);
                        tgt.setPixel(i, j, BLUE, MAXRGB);
                    }
                }
            }
        }
    }
}

/*-----------------------------------------------------------------------**/

void createHistogram(image &src, roi rect, vector<int> &vec) {
    int total_pixel = rect.sx * rect.sy;
    // number of pixels
    for (int i = rect.y; i < rect.y+rect.sy; i++) {
        for (int j = rect.x; j < rect.x+rect.sx; j++) {
            vec[src.getPixel(i, j)] += 1;
        }
    }
}

void plotHistogram(vector<int> vec) {
    int len = vec.size();
    for (int i = 0; i < 10; i++) {
        cout << "  " << i << ": ";
        for (int j = 0; j < vec[i]/5; j++) {
            cout << "*";
        }
        cout << endl;
    }
    for (int i = 10; i < 100; i++) {
        cout << " " << i << ": ";
        for (int j = 0; j < vec[i]/5; j++) {
            cout << "*";
        }
        cout << endl;
    }
    for (int i = 100; i < len; i++) {
        cout << i << ": ";
        for (int j = 0; j < vec[i]/5; j++) {
            cout << "*";
        }
        cout << endl;
    }
}

void utility::histogramStretching(image &src, roi rect) {
    vector<int> nk(MAXRGB+1, 0); 
    createHistogram(src, rect, nk);
    plotHistogram(nk);
}