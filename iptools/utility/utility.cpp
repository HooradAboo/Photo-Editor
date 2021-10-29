#include "utility.h"

#define MAXLEN 256
#define MAXRGB 255
#define MINRGB 0
#define H_SIZE 300

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

void rgb2hsi (image &rgb, image &hsi) {
    hsi.resize(rgb.getNumberOfRows(), rgb.getNumberOfColumns());

    int red, green, blue;
    double rgb_sum;

    double r, g, b;
    double hue, saturation, intensity;

    double theta;

    for (int i = 0; i < rgb.getNumberOfRows(); i++) {
        for (int j = 0; j < rgb.getNumberOfColumns(); j++) {
            // cout << "RGB: (" << rgb.getPixel(i, j, RED) << ", " << rgb.getPixel(i, j, GREEN) << ", " << rgb.getPixel(i, j, BLUE) << ")" << endl;

            red = rgb.getPixel(i, j, RED);
            green = rgb.getPixel(i, j, GREEN);
            blue = rgb.getPixel(i, j, BLUE);

            // cout << "RGB: (" << red << ", " << green << ", " << blue << ")" << endl;

            rgb_sum = red + green + blue;

            // normalizeing RGB value
            r = red / rgb_sum;
            g = green / rgb_sum;
            b = blue / rgb_sum;

            // cout << "rgb: (" << r << ", " << g << ", " << b << ")" << endl;

            // calculate Hue
            theta = acos ((0.5 * ((r - g) + (r - b))) / (sqrt (pow ((r - g), 2) + (r - b) * (g - b))));
            // cout << "theta: " << theta << endl;
            // theta = acos ((0.5 * ((r - g) + (r - b))) / double (sqrt (pow ((r - g), 2) + (r - b) * (g - b))));
            if (b <= g) {
                hue = theta;
            } else if (b > g) {
                hue = (2 * M_PI) - theta;
            }
            hue *= 180 / M_PI;
            
            // calculate Saturation
            saturation = 1 - (3 * min(r, min(g, b)));
            saturation *= 100;

            // calculate Intensity
            intensity = rgb_sum / (3.0 * 255);
            intensity *= 100;


            

            // save Hue, Saturation and Intensity as image (it is not a real image).
            hsi.setPixel(i, j, RED, hue);
            hsi.setPixel(i, j, GREEN, saturation);
            hsi.setPixel(i, j, BLUE, intensity); 

            // cout << "HSI: (" << hue << ", " << saturation << ", " << intensity << ")" << endl;       
        }
    }
}

void hsi2rgb (image &hsi, image &rgb) {
    rgb.resize(hsi.getNumberOfRows(), hsi.getNumberOfColumns());

    double hue, saturation, intensity;
    double h, s, ii;
    double tmp;

    double red, green, blue;

    bool flag = 0;

    for (int i = 0; i < hsi.getNumberOfRows(); i++) {
        for (int j = 0; j < hsi.getNumberOfColumns(); j++) {

            hue = hsi.getPixel(i, j, RED);
            saturation = hsi.getPixel(i, j, GREEN);
            intensity = hsi.getPixel(i, j, BLUE);

            // cout << "HSI: (" << hue << ", " << saturation << ", " << intensity << ")" << endl;
            
            // normalizeing HSI value
            h = hue * M_PI / 180;
            s = saturation / 100;
            ii = intensity / 100;

            // cout << "hsi: (" << h << ", " << s << ", " << ii << ")" << endl;

            // cout << "range: (" << 2 * M_PI / 3 << ", " << 4 * M_PI /4 << ", " << 2 * M_PI << ")" << endl;

            // calculate RGB
            if (0 <= h && h < 2 * M_PI / 3) { 
                blue = ii * (1 - s);
                red = ii * (1 + ((s * cos(h)) / (cos(M_PI / 3 - h))));
                green = 3 * ii - (blue + red);
            } else if (2 * M_PI / 3 <= h && h < 4 * M_PI /3) {
                h = h - 2 * M_PI / 3;
                red = ii * (1 - s);
                green = ii * (1 + ((s * cos(h)) / (cos(M_PI / 3 - h))));
                blue = 3 * ii - (red + green);
            } else if (4 * M_PI / 3 <= h && h <= 2 * M_PI) {
                h = h - 4 * M_PI / 3;
                green = ii * (1 - s);
                blue = ii * (1 + ((s * cos(h)) / (cos(M_PI / 3 - h))));
                red = 3 * ii - (green + blue);
            }

            red = round (red * 255);
            green = round (green * 255);
            blue = round (blue * 255);
            
            // cout << "rgb: (" << red << ", " << green << ", " << blue << ")" << endl;

            // save Hue, Saturation and Intensity as image (it is not a real image).
            rgb.setPixel(i, j, RED, red);
            rgb.setPixel(i, j, GREEN, green);
            rgb.setPixel(i, j, BLUE, blue);     
        }
    }
}


/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, char *input) {
    // create target image (it is empty)
    copyImage (src, tgt);

    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " "); 

    roi rect;                               // Region of Intrest
    int intensity;                          // added intensity value
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3) ; r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse input intensity
        intensity = atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Intensity: " << intensity << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + intensity));
            }
        }
    }
}


/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, char *input) {
    // create target image (it is empty)
    copyImage (src, tgt);

    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " "); 

    roi rect;                               // Region of Intrest
    int threshold;                          // threshold
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse input threshold
        threshold = atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Threshold: " << threshold << endl;

        // set new pixel value in output image
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
void utility::adjustBrightness(image &src, image &tgt, char *input) {
    // create target image (it is empty)
    copyImage (src, tgt);

    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " "); 

    roi rect;                               // Region of Intrest
    int threshold;                          // threshold value
    int value1, value2;                     // add value1 to I(x,y) > threshold and sub value2 to I(x,y) < threshold
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3) ; r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse input threshold
        threshold = atoi(strtok(NULL, " "));

        // parse input values
        value1 = atoi(strtok(NULL, " "));
        value2 = atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Threshold: " << threshold << endl;
        cout << "Value 1: " << value1 << endl;
        cout << "Value 2: " << value2 << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                if (src.getPixel(i, j) > threshold) {
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + value1));
                } else if (src.getPixel(i, j) < threshold) {
                    tgt.setPixel(i, j, checkValue(src.getPixel(i, j) - value2));
                }
            }
        }
    }
}


/*-----------------------------------------------------------------------**/
void utility::smoothing(image &src, image &tgt, char *input) {
     // create target image (it is empty)
    copyImage (src, tgt);
    
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " "); 

    roi rect;                               // Region of Intrest
    int window;                             // window size
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    int avg;                                // average of window's elements
    int sum;                                // sum of window's elements

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse input window size
        window = atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Window: " << window << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                sum = 0;
                for (int wr = 0; wr < window; wr++) {
                    for (int wc = 0; wc < window; wc++) {
                        sum += tgt.getPixel(i + wr, j + wc);
                    }
                }
                avg = sum / pow(window, 2);
                tgt.setPixel(i + window/2, j + window/2, checkValue(avg));
            }
        }
    }
}


/*-----------------------------------------------------------------------**/
void utility::colorAdjustBrightness(image &src, image &tgt, char *input) {
    // create target image (it is empty)
    copyImage (src, tgt);

    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " "); 

    roi rect;                               // Region of Intrest
    int more_c;                             // More_C threshold
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3) ; r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse input more_c
        more_c = atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "More-C: " << more_c << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                tgt.setPixel(i, j, RED, checkValue(src.getPixel(i, j, RED) * more_c));
                tgt.setPixel(i, j, GREEN, checkValue(src.getPixel(i, j, GREEN) + more_c));
                tgt.setPixel(i, j, BLUE, checkValue(src.getPixel(i, j, BLUE) + more_c));
            }
        }
    }
}


/*-----------------------------------------------------------------------**/
void utility::colorBinarize(image &src, image &tgt, char *input) {
    // create target image (it is empty)
    copyImage (src, tgt);

    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " "); 

    roi rect;                               // Region of Intrest
    int tc, tc_original;                          // threshold value
    int cr, cg, cb;                         // 
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3) ; r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse input threshold
        tc = atoi(strtok(NULL, " "));

        // parse input values
        cr = atoi(strtok(NULL, " "));
        cg = atoi(strtok(NULL, " "));
        cb = atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Threshold: " << tc << endl;
        cout << "C(R,G,B): (" << cr << "," << cg << "," << cb << ")" << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                tc_original = sqrt (pow(cr-src.getPixel(i, j, RED),2) + pow(cg-src.getPixel(i, j, GREEN),2) + pow(cb-src.getPixel(i, j, BLUE),2));
                if (tc_original < tc){
                    tgt.setPixel(i, j, RED, MAXRGB);
                    tgt.setPixel(i, j, GREEN, MINRGB);
                    tgt.setPixel(i, j, BLUE, MINRGB);
                } else if (tc <= tc_original && tc_original <= 2*tc) {
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


/*-----------------------------------------------------------------------**/
void utility::histogram(image &src, image &tgt, char *input) {
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " "); 

    roi rect;                               // Region of Intrest
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;

        int intensity[256];
        for (int i = 0; i < 256; i++) {
            intensity[i] = 0;
        }

        // create vector of histogram
        int pixel;
        for (int i = 0; i < rect.sy; i++) {
            for (int j = 0; j < rect.sx; j++) {
                pixel = src.getPixel(i, j);
                intensity[pixel] = intensity[pixel] + 1;
            }
        }

        // normalize vector of histogram
        double total_pixel = rect.sx * rect.sy;
        // cout << (total_pixel / 100.0) << endl;
        for (int i = 0; i < 256; i++) {
            // cout << intensity[i] << " ";
            intensity[i] = double (intensity[i]) / (total_pixel / 10000.0);
            // cout << intensity[i] << endl;
            // cout << double (intensity[i]) / (total_pixel / 10000.0) << endl;
        }

        // draw histogram
        tgt.resize(256, 256);
        for (int i = 0; i < tgt.getNumberOfRows(); i++) {
            for (int j = 0; j < src.getNumberOfColumns(); j++) {
                if (j < intensity[i]) {
                    tgt.setPixel(i, j, 255);
                }
            }
        }

        if (r == 0) {
            tgt.save("hist_1stROI.pgm");
        } else if (r == 1) {
            tgt.save("hist_2edROI.pgm");
        } else if (r == 2) {
            tgt.save("hist_3edROI.pgm");
        }
    }
}


/*-----------------------------------------------------------------------**/
void utility::colorHistogram(image &src, image &tgt, char *input) {
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " "); 

    roi rect;                               // Region of Intrest
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;

        int intensityR[256], intensityG[256], intensityB[256];
        for (int i = 0; i < 256; i++) {
            intensityR[i] = 0;
            intensityG[i] = 0;
            intensityB[i] = 0;
        }

        // create vector of histogram
        int pixel;
        for (int i = 0; i < rect.sy; i++) {
            for (int j = 0; j < rect.sx; j++) {
                pixel = src.getPixel(i, j, RED);
                intensityR[pixel] = intensityR[pixel] + 1;

                pixel = src.getPixel(i, j, GREEN);
                intensityG[pixel] = intensityG[pixel] + 1;

                pixel = src.getPixel(i, j, BLUE);
                intensityB[pixel] = intensityB[pixel] + 1;
            }
        }

        // normalize vector of histogram
        double total_pixel = rect.sx * rect.sy;
        // cout << (total_pixel / 100.0) << endl;
        for (int i = 0; i < 256; i++) {
            // cout << intensityR[i] << " ";
            intensityR[i] = double (intensityR[i]) / (total_pixel / 10000.0);
            intensityG[i] = double (intensityG[i]) / (total_pixel / 10000.0);
            intensityB[i] = double (intensityB[i]) / (total_pixel / 10000.0);
            // cout << intensityR[i] << endl;
            // cout << double (intensityR[i]) / (total_pixel / 10000.0) << endl;
        }

        // draw histogram
        image tgtR, tgtG, tgtB;
        tgtR.resize(256, 256);
        tgtG.resize(256, 256);
        tgtB.resize(256, 256);
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 256; j++) {
                if (j < intensityR[i]) {
                    tgtR.setPixel(i, j, RED, 255);
                }
                if (j < intensityG[i]) {
                    tgtG.setPixel(i, j, GREEN, 255);
                }
                if (j < intensityB[i]) {
                    tgtB.setPixel(i, j, BLUE, 255);
                }
            }
        }

        if (r == 0) {
            tgtR.save("histR_1stROI.ppm");
            tgtG.save("histG_1stROI.ppm");
            tgtB.save("histB_1stROI.ppm");
        } else if (r == 1) {
            tgtR.save("histR_2edROI.ppm");
            tgtG.save("histG_2edROI.ppm");
            tgtB.save("histB_2edROI.ppm");
        } else if (r == 2) {
            tgtR.save("histR_3edROI.ppm");
            tgtG.save("histG_3edROI.ppm");
            tgtB.save("histB_3edROI.ppm");
        }
    }
}


/*-----------------------------------------------------------------------**/
void utility::stretching(image &src, image &tgt, char *input) {
    // create target image (it is empty)
    copyImage (src, tgt);
    
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    interval range;                         // range parameter (a, b)
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    int pixel, new_pixel;                   // I(i, j) and I'(i, j)

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse input range
        range.a = atoi(strtok(NULL, " "));
        range.b = atoi(strtok(NULL, " "));

        // find min and max intensity in the ROI
        interval roi_range;
        roi_range.a = 255;          // minimum intencity
        roi_range.b = 0;          // maximum intencity
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                if (src.getPixel(i, j) != 0 && src.getPixel(i, j) != 255) {
                    if (src.getPixel(i, j) < roi_range.a) {
                        roi_range.a = src.getPixel(i, j);
                    }
                    if (roi_range.b < src.getPixel(i, j)) {
                        roi_range.b = src.getPixel(i, j);
                    }
                } 
            }
        }

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(SX,SY): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "(" << roi_range.a << "," << roi_range.b << ") --> (" << range.a << "," << range.b << ")" << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                pixel = src.getPixel(i, j);
                if (0 <= pixel && pixel <= range.a) {
                    tgt.setPixel(i, j, 0);
                } else if (range.a < pixel && pixel < range.b) {
                    new_pixel = round((pixel - roi_range.a) * double ((range.b - range.a)/double(roi_range.b - roi_range.a)) + range.a);
                    tgt.setPixel(i, j, checkValue(new_pixel));
                } else if (range.b <= pixel && pixel <= 255) {
                    tgt.setPixel(i, j, 255);
                }
            }
        }
    }
}


/*-----------------------------------------------------------------------**/
void utility::thresholdStretching(image &src, image &tgt, char *input) {
    // create target image (it is empty)
    copyImage (src, tgt);
    
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    int threhold;                           // modification threshold
    interval dark_range, bright_range;;     // range parameters (a1, b1)  and (a2, b) 
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    int pixel, new_pixel;                   // I(i, j) and I'(i, j)

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse threshold
        threhold = atoi(strtok(NULL, " "));

        // parse input range dark pixels
        dark_range.a = atoi(strtok(NULL, " "));
        dark_range.b = atoi(strtok(NULL, " "));

        // parse input range bright pixels
        bright_range.a = atoi(strtok(NULL, " "));
        bright_range.b = atoi(strtok(NULL, " "));

        // find min and max intensity in the ROI
        interval roi_dark_range, roi_bright_range;

        roi_dark_range.a = 255;           // minimum intencity in background
        roi_dark_range.b = 0;             // maximum intencity in background

        roi_bright_range.a = 255;         // minimum intencity in forground
        roi_bright_range.b = 0;           // maximum intencity in forground

        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                // remove the 0 and 255 intensity
                if (src.getPixel(i, j) != 0 && src.getPixel(i, j) != 255) {
                    // find min and max intensity in background
                    if (src.getPixel(i, j) < threhold) {
                        if (src.getPixel(i, j) < roi_dark_range.a) {
                            roi_dark_range.a = src.getPixel(i, j);
                        }
                        if (roi_dark_range.b < src.getPixel(i, j)) {
                            roi_dark_range.b = src.getPixel(i, j);
                        }
                    }
                    // find min and max intensity in background
                    else if (threhold <= src.getPixel(i, j)) {
                        if (src.getPixel(i, j) < roi_bright_range.a) {
                            roi_bright_range.a = src.getPixel(i, j);
                        }
                        if (roi_bright_range.b < src.getPixel(i, j)) {
                            roi_bright_range.b = src.getPixel(i, j);
                        }
                    } 
                } 
            }
        }

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(SX,SY): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Threshold: " << threhold << endl;
        cout << "(" << roi_dark_range.a << "," << roi_dark_range.b << ") --> (" << dark_range.a << "," << dark_range.b << ")" << endl;
        cout << "(" << roi_bright_range.a << "," << roi_bright_range.b << ") --> (" << bright_range.a << "," << bright_range.b << ")" << endl;


        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                pixel = src.getPixel(i, j);
                if (pixel > threhold) {                     // background
                    if (pixel <= dark_range.a) {
                        tgt.setPixel (i, j, 0);
                    } else if (dark_range.a < pixel && pixel < dark_range.b) {
                        new_pixel = round((pixel - roi_dark_range.a) * double ((dark_range.b - dark_range.a)/double(roi_dark_range.b - roi_dark_range.a)) + dark_range.a);
                        tgt.setPixel (i, j, new_pixel);
                    } else if (dark_range.b <= pixel) {
                        tgt.setPixel (i, j, 255);
                    }
                } else if (threhold >= pixel) {             // forground
                    if (pixel <= bright_range.a) {
                        tgt.setPixel (i, j, 0);
                    } else if (bright_range.a < pixel && pixel < bright_range.b) {
                        new_pixel = round((pixel - roi_bright_range.a) * double ((bright_range.b - bright_range.a)/double(roi_bright_range.b - roi_bright_range.a)) + bright_range.a);
                        tgt.setPixel (i, j, new_pixel);
                    } else if (bright_range.b <= pixel) {
                        tgt.setPixel (i, j, 255);
                    }
                }
            }
        }
    }
}


/*-----------------------------------------------------------------------**/
void utility::channelStretching(image &src, image &tgt, char *input) {
    // create target image (it is empty)
    copyImage (src, tgt);
    
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    interval range;                         // range parameter (a, b)
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI
    channel rgb;                            // the target channel

    int pixel, new_pixel;                   // I(i, j) and I'(i, j)
    char *pch;

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input roi
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = atoi(strtok(NULL, " "));
        rect.sy = atoi(strtok(NULL, " "));

        // parse target channel  
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

        // parse input range
        range.a = atoi(strtok(NULL, " "));
        range.b = atoi(strtok(NULL, " "));

        // // print input data
        // cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        // cout << "(SX,SY): (" << rect.sx << "," << rect.sy << ")" << endl;
        // cout << "(a,b): (" << range.a << "," << range.b << ")" << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                pixel = src.getPixel(i, j);                         // I(a, b)
                if (0 <= pixel && pixel <= range.a) {
                    tgt.setPixel(i, j, rgb, 0);                     // I'(a, b) = 0
                } else if (range.a < pixel && pixel < range.b) {
                    new_pixel = round(double(pixel * ((range.b - range.a)/double(255 - 0))) + range.a);
                    tgt.setPixel(i, j, rgb, checkValue(new_pixel)); // I'(a, b)
                } else if (range.b <= pixel && pixel <= 255) {
                    tgt.setPixel(i, j, rgb, 255);                   // I'(a, b) = 255
                }
            }
        }

        // // create and save image histogram
        // createHistogram(src, rect);
        // createHistogram(tgt, rect);
    }
}


/*-----------------------------------------------------------------------**/
void utility::colorStretching(image &src, image &tgt, char *input) {
    // create target image (it is empty)
    copyImage (src, tgt);

    // image hsi;
    // rgb2Hsi (src, hsi);
    // copyImage (hsi, src);

    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    interval range;                         // range parameter (a, b)
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    int pixelR, pixelG, pixelB, new_pixel;  // redI(i, j) and greenI(i, j) and blueI(i, j) and I'(i,j)

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input roi
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = atoi(strtok(NULL, " "));
        rect.sy = atoi(strtok(NULL, " "));

        // parse input range
        range.a = atoi(strtok(NULL, " "));
        range.b = atoi(strtok(NULL, " "));

        // // print input data
        // cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        // cout << "(SX,SY): (" << rect.sx << "," << rect.sy << ")" << endl;
        // cout << "(a,b): (" << range.a << "," << range.b << ")" << endl;
        
        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                // stretch Red Channel
                pixelR = src.getPixel(i, j, RED);                       // RI(i,j)
                if (0 <= pixelR && pixelR <= range.a) {
                    tgt.setPixel(i, j, RED, 0);                         // RI'(i,j) = 0
                } else if (range.a < pixelR && pixelR < range.b) {
                    new_pixel = round(double(pixelR * ((range.b - range.a)/double(255 - 0))) + range.a);
                    tgt.setPixel(i, j, RED, checkValue(new_pixel));     // RI'(i,j)
                } else if (range.b <= pixelR && pixelR <= 255) {
                    tgt.setPixel(i, j, RED, 255);                       // RI'(i,j) = 255
                }
                
                // stretch Green Channel
                pixelG = src.getPixel(i, j, GREEN);                     // GI(i,j)
                if (0 <= pixelG && pixelG <= range.a) {
                    tgt.setPixel(i, j, GREEN, 0);                       // GI'(i,j) = 0
                } else if (range.a < pixelG && pixelG < range.b) {
                    new_pixel = round(double(pixelG * ((range.b - range.a)/double(255 - 0))) + range.a);
                    tgt.setPixel(i, j, GREEN, checkValue(new_pixel));   // GI'(i,j)   
                } else if (range.b <= pixelG && pixelG <= 255) {
                    tgt.setPixel(i, j, GREEN, 255);                     // GI'(i,j) = 255
                }

                // stretch Blue Channel
                pixelR = src.getPixel(i, j, BLUE);                      // BI(i,j)
                if (0 <= pixelB && pixelB <= range.a) {
                    tgt.setPixel(i, j, BLUE, 0);                        // BI'(i,j) = 0
                } else if (range.a < pixelB && pixelB < range.b) {
                    new_pixel = round(double(pixelB * ((range.b - range.a)/double(255 - 0))) + range.a);
                    tgt.setPixel(i, j, BLUE, checkValue(new_pixel));    // BI'(i,j)
                } else if (range.b <= pixelB && pixelB <= 255) {
                    tgt.setPixel(i, j, BLUE, 255);                      // BI'(i,j) = 255
                }
            }
        }

        // // create and save image histogram
        // createColorHistogram(src, rect);
        // createColorHistogram(tgt, rect);
    }
}


/*-----------------------------------------------------------------------**/
void utility::hueStretching(image &src, image &tgt, char *input) {
    // convert RGB to HSI
    image hsi;
    rgb2hsi (src, hsi);

    // create target image (it is empty)
    image tmp;
    copyImage (hsi, tmp);
    
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    interval range;                         // range parameter (a, b)
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    int pixel, new_pixel;                   // I(i, j) and I'(i, j)

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = atoi(strtok(NULL, " "));
        rect.sy = atoi(strtok(NULL, " "));

        // parse input range
        range.a = atoi(strtok(NULL, " "));
        range.b = atoi(strtok(NULL, " "));

        // print input date
        // cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        // cout << "(SX,SY): (" << rect.sx << "," << rect.sy << ")" << endl;
        // cout << "(a,b): (" << range.a << "," << range.b << ")" << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                // if (tmp.getPixel(i, j, GREEN) > 100)
                //     cout << tmp.getPixel(i, j, GREEN) << " ";
                pixel = hsi.getPixel(i, j);                     // I(i, j)
                if (0 <= pixel && pixel <= range.a) {
                    tmp.setPixel(i, j, 0);                      // I'(i, j) = 0
                } else if (range.a < pixel && pixel < range.b) {
                    new_pixel = round(double(pixel * ((range.b - range.a)/double(255 - 0))) + range.a);
                    tmp.setPixel(i, j, checkValue(new_pixel));  // I'(i, j)
                } else if (range.b <= pixel && pixel <= 255) {
                    tmp.setPixel(i, j, 255);                    // I'(i, j) = 255
                }
            }
        }

        image rgb;
        hsi2rgb (tmp, rgb);
        copyImage (rgb, tgt);

        // // create and save image histogram
        // createHistogram(src, rect);
        // createHistogram(tgt, rect);
    }
}


void utility::hueSaturationStretching(image &src, image &tgt, char *input) {
    // convert RGB to HSI
    image hsi;
    rgb2hsi (src, hsi);

    // create target image (it is empty)
    image tmp;
    copyImage (hsi, tmp);
    
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    interval hue_range, saturation_range;   // Hue range parameter (a1, b1)
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    int pixelH, pixelS, new_pixel;          // HI(i, j) and SI(i, j) and I'(i, j)

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = atoi(strtok(NULL, " "));
        rect.sy = atoi(strtok(NULL, " "));

        // parse input range
        hue_range.a = atoi(strtok(NULL, " "));
        hue_range.b = atoi(strtok(NULL, " "));

        saturation_range.a = atoi(strtok(NULL, " "));
        saturation_range.b = atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(SX,SY): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "(a1,b1): (" << hue_range.a << "," << hue_range.b << ")" << endl;
        cout << "(a2,b2): (" << saturation_range.a << "," << saturation_range.b << ")" << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                // stretch Hue
                pixelH = src.getPixel(i, j, RED);                       // HI(i,j)
                if (0 <= pixelH && pixelH <= hue_range.a) {
                    tgt.setPixel(i, j, RED, 0);                         // HI'(i,j) = 0
                } else if (hue_range.a < pixelH && pixelH < hue_range.b) {
                    new_pixel = round(double(pixelH * ((hue_range.b - hue_range.a)/double(255 - 0))) + hue_range.a);
                    tgt.setPixel(i, j, RED, checkValue(new_pixel));     // HI'(i,j)
                } else if (hue_range.b <= pixelH && pixelH <= 255) {
                    tgt.setPixel(i, j, RED, 255);                       // HI'(i,j) = 255
                }
                
                // stretch Saturation
                pixelS = src.getPixel(i, j, GREEN);                     // SI(i,j)
                if (0 <= pixelS && pixelS <= saturation_range.a) {
                    tgt.setPixel(i, j, GREEN, 0);                       // SI'(i,j) = 0
                } else if (saturation_range.a < pixelS && pixelS < saturation_range.b) {
                    new_pixel = round(double(pixelS * ((saturation_range.b - saturation_range.a)/double(255 - 0))) + saturation_range.a);
                    tgt.setPixel(i, j, GREEN, checkValue(new_pixel));   // SI'(i,j)   
                } else if (saturation_range.b <= pixelS && pixelS <= 255) {
                    tgt.setPixel(i, j, GREEN, 255);                     // SI'(i,j) = 255
                }

            }
        }

        image rgb;
        hsi2rgb (tmp, rgb);
        copyImage (rgb, tgt);

        // // create and save image histogram
        // createHistogram(src, rect);
        // createHistogram(tgt, rect);
    }
}


void utility::hueSaturationIntensityStretching(image &src, image &tgt, char *input) {
    // convert RGB to HSI
    image hsi;
    rgb2hsi (src, hsi);

    // create target image (it is empty)
    image tmp;
    copyImage (hsi, tmp);
    
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                                                       // Region of Intrest
    interval hue_range, saturation_range, intensity_range;          // Hue range parameter (a1, b1)
    int roi_num = atoi(strtok(NULL, " "));                          // number of RoI

    int pixelH, pixelS, pixelI, new_pixel;                          // HI(i, j) and SI(i, j) and II(i, j) and I'(i, j)

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = atoi(strtok(NULL, " "));
        rect.sy = atoi(strtok(NULL, " "));

        // parse input range
        hue_range.a = atoi(strtok(NULL, " "));
        hue_range.b = atoi(strtok(NULL, " "));

        saturation_range.a = atoi(strtok(NULL, " "));
        saturation_range.b = atoi(strtok(NULL, " "));

        intensity_range.a = atoi(strtok(NULL, " "));
        intensity_range.b = atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(SX,SY): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "(a1,b1): (" << hue_range.a << "," << hue_range.b << ")" << endl;
        cout << "(a2,b2): (" << saturation_range.a << "," << saturation_range.b << ")" << endl;
        cout << "(a3,b3): (" << intensity_range.a << "," << intensity_range.b << ")" << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
                // stretch Hue
                pixelH = src.getPixel(i, j, RED);                       // HI(i,j)
                if (0 <= pixelH && pixelH <= hue_range.a) {
                    tgt.setPixel(i, j, RED, 0);                         // HI'(i,j) = 0
                } else if (hue_range.a < pixelH && pixelH < hue_range.b) {
                    new_pixel = round(double(pixelH * ((hue_range.b - hue_range.a)/double(255 - 0))) + hue_range.a);
                    tgt.setPixel(i, j, RED, checkValue(new_pixel));     // HI'(i,j)
                } else if (hue_range.b <= pixelH && pixelH <= 255) {
                    tgt.setPixel(i, j, RED, 255);                       // HI'(i,j) = 255
                }
                
                // stretch Saturation
                pixelS = src.getPixel(i, j, GREEN);                     // SI(i,j)
                if (0 <= pixelS && pixelS <= saturation_range.a) {
                    tgt.setPixel(i, j, GREEN, 0);                       // SI'(i,j) = 0
                } else if (saturation_range.a < pixelS && pixelS < saturation_range.b) {
                    new_pixel = round(double(pixelS * ((saturation_range.b - saturation_range.a)/double(255 - 0))) + saturation_range.a);
                    tgt.setPixel(i, j, GREEN, checkValue(new_pixel));   // SI'(i,j)   
                } else if (saturation_range.b <= pixelS && pixelS <= 255) {
                    tgt.setPixel(i, j, GREEN, 255);                     // SI'(i,j) = 255
                }

                // stretch Intensity
                pixelI = src.getPixel(i, j, BLUE);                     // II(i,j)
                if (0 <= pixelI && pixelI <= intensity_range.a) {
                    tgt.setPixel(i, j, BLUE, 0);                       // II'(i,j) = 0
                } else if (intensity_range.a < pixelI && pixelI < intensity_range.b) {
                    new_pixel = round(double(pixelI * ((intensity_range.b - intensity_range.a)/double(255 - 0))) + intensity_range.a);
                    tgt.setPixel(i, j, BLUE, checkValue(new_pixel));   // II'(i,j)   
                } else if (intensity_range.b <= pixelI && pixelI <= 255) {
                    tgt.setPixel(i, j, BLUE, 255);                     // II'(i,j) = 255
                }
            }
        }

        image rgb;
        hsi2rgb (tmp, rgb);
        copyImage (rgb, tgt);

        // // create and save image histogram
        // createHistogram(src, rect);
        // createHistogram(tgt, rect);
    }
}


/*-----------------------------------------------------------------------**/
void utility::edgeDetection(image &src, image &tgt, char *input) {
    // create target image (it is empty)
    copyImage (src, tgt);
    
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " "); 

    roi rect;                               // Region of Intrest
    int window;                             // window size
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    vector<vector<int>> sobel;              // sobel matrix

    int magnitude;                          // gradiant magnitude
    int dx, dy;                             // derivative in x and y direction

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse input window size
        window = atoi(strtok(NULL, " "));

        // create kernel matrix based on the window size
        if (window == 3) {
            sobel = {
                {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}
            };
            
        } else if (window == 5) {
            sobel = {
                // {2, 1, 0, -1, -2},
                // {2, 1, 0, -1, -2},
                // {4, 2, 0, -2, -4},
                // {2, 1, 0, -1, -2},
                // {2, 1, 0, -1, -2}
                { -5,  -4,  0,   4,  5},
                { -8, -10,  0,  10,  8},
                {-10, -20,  0,  20, 10},
                { -8, -10,  0,  10,  8},
                { -5,  -4,  0,   4,  5}
            };
        }

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Window: " << window << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
               dx = 0; 
               dy = 0;
                for (int wr = 0; wr < window; wr++) {
                    for (int wc = 0; wc < window; wc++) {
                        dx = dx + (src.getPixel(i + wr, j + wc) * sobel[wr][wc]);
                        dy = dy + (src.getPixel(i + wc, j + wr) * sobel[wr][wc]);
                    }
                }
                magnitude = sqrt(pow(dx, 2) + pow(dy, 2));
                tgt.setPixel(i + window/2, j + window/2, magnitude);
            }
        }
    }
}


/*-----------------------------------------------------------------------**/
void utility::thresholdEdgeDetection(image &src, image &tgt, char *input) {
    // create target image (it is empty)
    copyImage (src, tgt);
    
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " "); 

    roi rect;                               // Region of Intrest
    int window;                             // window size
    int threshold;                           // threshold
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    vector<vector<int>> sobel;              // sobel matrix

    int magnitude;                          // gradiant magnitude
    int dx, dy;                             // derivative in x and y direction

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse window size and threshold
        window = atoi(strtok(NULL, " "));
        threshold = atoi(strtok(NULL, " "));

        // create kernel matrix based on the window size
        if (window == 3) {
            sobel = {
                {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}
            };


        } else if (window == 5) {
            sobel = {
                // {2, 1, 0, -1, -2},
                // {2, 1, 0, -1, -2},
                // {4, 2, 0, -2, -4},
                // {2, 1, 0, -1, -2},
                // {2, 1, 0, -1, -2}
                { -5,  -4,  0,   4,  5},
                { -8, -10,  0,  10,  8},
                {-10, -20,  0,  20, 10},
                { -8, -10,  0,  10,  8},
                { -5,  -4,  0,   4,  5}
            };
        }

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Window: " << window << endl;
        cout << "Threshold: " << threshold << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
               dx = 0; 
               dy = 0;
                for (int wr = 0; wr < window; wr++) {
                    for (int wc = 0; wc < window; wc++) {
                        dx = dx + (src.getPixel(i + wr, j + wc) * sobel[wr][wc]);
                        dy = dy + (src.getPixel(i + wc, j + wr) * sobel[wr][wc]);
                    }
                }
                magnitude = sqrt(pow(dx, 2) + pow(dy, 2));

                if (magnitude <= threshold) {
                    tgt.setPixel(i + window/2, j + window/2, MINRGB);
                } else if (magnitude > threshold) {
                    tgt.setPixel(i + window/2, j + window/2, MAXRGB);
                }
            }
        }
    }
}


/*-----------------------------------------------------------------------**/
void utility::directionEdgeDetection(image &src, image &tgt, char *input) {
    // create target image (it is empty)
    copyImage (src, tgt);
    
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " "); 

    roi rect;                               // Region of Intrest
    int window;                             // window size
    int threshold;                           // threshold
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    vector<vector<int>> sobel;              // sobel matrix
    int distance;

    int magnitude;                          // gradiant magnitude
    double theta;
    int dx, dy;                             // derivative in x and y direction

    for (int r = 0; r < min(roi_num, 3); r++){
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse window size and threshold
        window = atoi(strtok(NULL, " "));
        threshold = atoi(strtok(NULL, " "));

        // create kernel matrix based on the window size
        if (window == 3) {
            sobel = {
                {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}
            };

            distance = 4;

        } else if (window == 5) {
            sobel = {
                // {2, 1, 0, -1, -2},
                // {2, 1, 0, -1, -2},
                // {4, 2, 0, -2, -4},
                // {2, 1, 0, -1, -2},
                // {2, 1, 0, -1, -2}
                { -5,  -4,  0,   4,  5},
                { -8, -10,  0,  10,  8},
                {-10, -20,  0,  20, 10},
                { -8, -10,  0,  10,  8},
                { -5,  -4,  0,   4,  5}
            };

            distance = 84;
        }

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Window: " << window << endl;
        cout << "Threshold: " << threshold << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            // cout << i << endl;
            for (int j = rect.x; j < rect.sx; j++) {
               dx = 0; 
               dy = 0;
                for (int wr = 0; wr < window; wr++) {
                    for (int wc = 0; wc < window; wc++) {
                        dx = dx + (src.getPixel(i + wr, j + wc) * sobel[wr][wc]);
                        dy = dy + (src.getPixel(i + wc, j + wr) * sobel[wr][wc]);
                    }
                }
                magnitude = sqrt(pow(dx, 2) + pow(dy, 2));
                if (dx != 0) {
                    theta = atan (dy/dx);
                    if (theta < 0) {
                        theta += (2 * M_PI);
                    }
                } else
                    theta = M_PI / 2;
                theta = round (theta * 180 / M_PI);
                // cout << theta << endl;
                if (theta <= 10 && 350 <= theta) {    
                    if (magnitude <= threshold) {
                        tgt.setPixel(i + window/2, j + window/2, MINRGB);
                    } else if (magnitude > threshold) {
                        tgt.setPixel(i + window/2, j + window/2, MAXRGB);
                    }
                } else if (35 <= theta && theta <= 55) {
                    if (magnitude <= threshold) {
                        tgt.setPixel(i + window/2, j + window/2, MINRGB);
                    } else if (magnitude > threshold) {
                        tgt.setPixel(i + window/2, j + window/2, MAXRGB);
                    }
                } else {
                    tgt.setPixel(i + window/2, j + window/2, MINRGB);
                }
            }
        }
    }
}


/*-----------------------------------------------------------------------**/
void utility::colorEdgeDetection(image &src, image &tgt, char *input) {
    // tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    copyImage (src, tgt);

    image hsi_src;                 // input and output image in HSI

    // convert input image from RGB to HSI
    rgb2hsi (src, hsi_src);

    // // create target image (it is empty)
    // copyImage (hsi_src, hsi_tgt);

    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " "); 

    roi rect;                               // Region of Intrest
    int window;                             // window size
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    vector<vector<int>> sobel;              // sobel matrix

    int magnitude, theta;                   // gradiant magnitude and direction
    int dx, dy;                             // derivative in x and y direction

    for (int r = 0; r < min(roi_num, 3); r++) {
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse input range
        window = atoi(strtok(NULL, " "));

        // create kernel matrix based on the window size
        if (window == 3) {
            sobel = {
                {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}
            };
        } else if (window == 5) {
            sobel = {
                // {2, 1, 0, -1, -2},
                // {2, 1, 0, -1, -2},
                // {4, 2, 0, -2, -4},
                // {2, 1, 0, -1, -2},
                // {2, 1, 0, -1, -2}
                { -5,  -4,  0,   4,  5},
                { -8, -10,  0,  10,  8},
                {-10, -20,  0,  20, 10},
                { -8, -10,  0,  10,  8},
                { -5,  -4,  0,   4,  5}
            };
        }

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "window: " << window << endl;

        // set new pixel value in output image
        for (int i = rect.y; i < rect.sy; i++) {
            for (int j = rect.x; j < rect.sx; j++) {
               dx = 0; 
               dy = 0;
                for (int wr = 0; wr < window; wr++) {
                    for (int wc = 0; wc < window; wc++) {
                        dx = dx + (hsi_src.getPixel(i + wr, j + wc, BLUE) * sobel[wr][wc]);
                        dy = dy + (hsi_src.getPixel(i + wc, j + wr, BLUE) * sobel[wr][wc]);
                    }
                }
                magnitude = sqrt(pow(dx, 2) + pow(dy, 2));
                // normlise the strength of gradiant to the range of 0 to 255
                // magnitude = magnitude / 1428 * 255;
                if (dx != 0)
                    theta = atan(dy / dx);
                tgt.setPixel(i + window/2, j + window/2, RED, checkValue(magnitude));
                tgt.setPixel(i + window/2, j + window/2, GREEN, 0);
                tgt.setPixel(i + window/2, j + window/2, BLUE, 0);
            }
        }
    }

    // hsi2rgb (hsi_tgt, tgt);
}