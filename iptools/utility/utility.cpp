#include "utility.h"

#define MAXLEN 256
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

void copyImage (image &src, image &tgt) {
    // Make a copy of origin image to target image
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    for (int i = 0; i < src.getNumberOfRows(); i++) {
        for (int j = 0; j < src.getNumberOfColumns(); j++) {
            tgt.setPixel(i, j, RED, src.getPixel(i, j, RED));
            tgt.setPixel(i, j, GREEN, src.getPixel(i, j, GREEN));
            tgt.setPixel(i, j, BLUE, src.getPixel(i, j, BLUE));
        }
    }
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

void plotHistogram(vector<int> vec, int pixels_num) {
    #define H_SIZE 256
    image hist;
    hist.resize(H_SIZE, H_SIZE);

    // normalize the number of each intensity
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
        vec[i] = int((vec[i] / double(pixels_num)) * 2560); 
    }

    for (int i = 0; i < H_SIZE; i++) {
        for (int j = 0; j < H_SIZE; j++) {
            int tmp = vec[i];
            if (j < vec[i]) {
                hist.setPixel(i, j, 255);
            } else {
                hist.setPixel(i, j, 0);
            }  
        }
    }
    char outfile[MAXLEN] = "hist.pgm";
    hist.save(outfile);
}


/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, vector<v_roi> rect) {
    if (checkRoI (rect)) {
        // // Make a copy of origin image to target image
        copyImage (src, tgt);
        
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
        copyImage (src, tgt);

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
        copyImage (src, tgt);

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
        copyImage (src, tgt);

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
        copyImage (src, tgt);

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
        copyImage (src, tgt);

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
void utility::stretching(image &src, image &tgt, char *input) {
    #define c  0
    #define d  255

    roi rect;
    interval range;
    channel rgb;

    int pixel, new_pixel;
    char *pch;
    copyImage (src, tgt);

    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    int roi_num = atoi(strtok(NULL, " "));
    for (int r = 0; r < roi_num; r++){
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = atoi(strtok(NULL, " "));
        rect.sy = atoi(strtok(NULL, " "));

        pch =  strtok(NULL, " ");
        if (strcmp(pch, "r")) {
            rgb = RED;
        } else if (strcmp(pch, "g")) {
            rgb = GREEN;
        } else if (strcmp(pch, "b")) {
            rgb = BLUE;
        } else {
            rgb = RED;
        }

        range.a = atoi(strtok(NULL, " "));
        range.b = atoi(strtok(NULL, " "));

        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(SX,SY): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "(a,b): (" << range.a << "," << range.b << ")" << endl;

        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                pixel = src.getPixel(i, j);
                // cout << "pix:" << pixel << endl;
                if (0 <= pixel && pixel <= range.a) {
                    tgt.setPixel(i, j, 0);
                } else if (range.a < pixel && pixel < range.b) {
                    new_pixel = round(double(pixel * ((range.b - range.a)/double(d - c))) + range.a);
                    tgt.setPixel(i, j, checkValue(new_pixel));
                    // cout << "new: " << tgt.getPixel(i,j) << endl;
                } else if (range.b <= pixel && pixel <= 255) {
                    tgt.setPixel(i, j, 255);
                }
            }
        }
        // createHistogram(src, rect);
        createHistogram(tgt, rect);
    }
}


/*-----------------------------------------------------------------------**/
void createColorHistogram(image &src, roi rect, vector<int> &vec) {
    vector<int> channelR (256, 0);
    for (int i = rect.y; i < rect.sy; i++) {
        for (int j = rect.x; j < rect.sx; j++) {
            channelR[src.getPixel(i, j, RED)] += 1;
        }
    }

    int total_pixel = (rect.sy - rect.y) * (rect.sx - rect.x);
    plotHistogram(channelR, total_pixel);
}

void utility::colorStretching(image &src, image &tgt, char *input) {
    #define c  0
    #define d  255

    roi rect;
    interval range;

    int pixelR, pixelG, pixelB, new_pixel;

    copyImage (src, tgt);

    // remove three first parameter (src, tgt, function name) from input string
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    int roi_num = atoi(strtok(NULL, " "));
    for (int r = 0; r < roi_num; r++){
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = atoi(strtok(NULL, " "));
        rect.sy = atoi(strtok(NULL, " "));

        range.a = atoi(strtok(NULL, " "));
        range.b = atoi(strtok(NULL, " "));

        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(SX,SY): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "(a,b): (" << range.a << "," << range.b << ")" << endl;

        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                // stretch Red Channel
                pixelR = src.getPixel(i, j, RED);
                // cout << "pix:" << pixel << endl;
                if (0 <= pixelR && pixelR <= range.a) {
                    tgt.setPixel(i, j, RED, 0);
                } else if (range.a < pixelR && pixelR < range.b) {
                    new_pixel = round(double(pixelR * ((range.b - range.a)/double(d - c))) + range.a);
                    tgt.setPixel(i, j, RED, checkValue(new_pixel));
                    // cout << "new: " << tgt.getPixel(i,j) << endl;
                } else if (range.b <= pixelR && pixelR <= 255) {
                    tgt.setPixel(i, j, RED, 255);
                }
                
                // stretch Green Channel
                pixelG = src.getPixel(i, j, GREEN);
                // cout << "pix:" << pixel << endl;
                if (0 <= pixelG && pixelG <= range.a) {
                    tgt.setPixel(i, j, GREEN, 0);
                } else if (range.a < pixelG && pixelG < range.b) {
                    new_pixel = round(double(pixelG * ((range.b - range.a)/double(d - c))) + range.a);
                    tgt.setPixel(i, j, GREEN, checkValue(new_pixel));
                    // cout << "new: " << tgt.getPixel(i,j) << endl;
                } else if (range.b <= pixelG && pixelG <= 255) {
                    tgt.setPixel(i, j, GREEN, 255);
                }

                // stretch Blue Channel
                pixelR = src.getPixel(i, j, BLUE);
                // cout << "pix:" << pixel << endl;
                if (0 <= pixelB && pixelB <= range.a) {
                    tgt.setPixel(i, j, BLUE, 0);
                } else if (range.a < pixelB && pixelB < range.b) {
                    new_pixel = round(double(pixelB * ((range.b - range.a)/double(d - c))) + range.a);
                    tgt.setPixel(i, j, BLUE, checkValue(new_pixel));
                    // cout << "new: " << tgt.getPixel(i,j) << endl;
                } else if (range.b <= pixelB && pixelB <= 255) {
                    tgt.setPixel(i, j, BLUE, 255);
                }
            }
        }

        // vector<int> nk(256, 0); 
        // createColorHistogram(src, rect, nk);
        // vector<int> mk(256, 0); 
        // createHistogram(tgt, rect, mk);

        // plotHistogram(nk);
        // plotHistogram(mk);
    }
}