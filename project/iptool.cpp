/************************************************************
 *															*
 * This sample project include three functions:				*
 * 1. Add intensity for gray-level image.					*
 *    Input: source image, output image name, value			*
 *															*
 * 2. Image thresholding: pixels will become black if the	*
 *    intensity is below the threshold, and white if above	*
 *    or equal the threhold.								*
 *    Input: source image, output image name, threshold		*
 *															*
 * 3. Image scaling: reduction/expansion of 2 for 			*
 *    the width and length. This project uses averaging 	*
 *    technique for reduction and pixel replication			*
 *    technique for expansion.								*
 *    Input: source image, output image name, scale factor	*
 *															*
 ************************************************************/

#include "../iptools/core.h"
#include <string.h>

using namespace std;

#define MAXLEN 256

bool isGray (char* name) {
    for (int i = 0; i <= strlen(name); i++) {
        // cout << name[i] << endl;
        if (name[i] == 'g') {
            // cout << "inja" << endl;
            if (name[i-2] == '.' && name[i-1] == 'p' && name[i+1] == 'm') {
                // cout << "inja1" << endl;
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char **argv) {
    image src, tgt;
    FILE *fp;
    char str[MAXLEN];
    char outfile[MAXLEN];
    char *pch;
    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Can't open file: %s\n", argv[1]);
        exit(1);
    }

    while (fgets(str, MAXLEN, fp) != NULL) {
        pch = strtok(str, " ");
        src.read(pch);

        // bool isGray_flag = isGray (pch);
        // if (isGray_flag) {
        //     // Input image is gray-scale
        //     cout << "gray" << endl;
        // } else {
        //     // Input image is RGB
        //     cout << "RGB" << endl;
        // }

        pch = strtok(NULL, " ");
        strcpy(outfile, pch);

        pch = strtok(NULL, " ");

        if (strcmp(pch, "add") == 0) {
            /* Add Intensity */
            vector<v_roi> vroi;
            v_roi rect;
            do {
                rect.value = atoi(strtok(NULL, " "));
                rect.x = atoi(strtok(NULL, " "));
                rect.y = atoi(strtok(NULL, " "));
                rect.sx = atoi(strtok(NULL, " "));
                rect.sy = atoi(strtok(NULL, " "));

                vroi.push_back(rect);
            } while (rect.sx != 0 && rect.sy != 0);
            vroi.pop_back();       
            utility::addGrey(src, tgt, vroi);

        } else if (strcmp(pch, "binarize") == 0) {
            /* Thresholding */
            vector<t_roi> vroi;
            t_roi rect;
            do {
                rect.threshold = atoi(strtok(NULL, " "));
                rect.x = atoi(strtok(NULL, " "));
                rect.y = atoi(strtok(NULL, " "));
                rect.sx = atoi(strtok(NULL, " "));
                rect.sy = atoi(strtok(NULL, " "));

                vroi.push_back(rect);
            } while (rect.sx != 0 && rect.sy != 0);
            vroi.pop_back(); 
            utility::binarize(src, tgt, vroi);

        } else if (strcmp(pch, "scale") == 0) {
            /* Image scaling */
            pch = strtok(NULL, " ");
            utility::scale(src, tgt, atof(pch));

        } else if (strcmp(pch, "brightness") == 0) {
            /* Image brightness */
            vector<vtv_roi> vroi;
            vtv_roi rect;
            do {
                rect.threshold = atoi(strtok(NULL, " "));
                rect.value1 = atoi(strtok(NULL, " "));
                rect.value2 = atoi(strtok(NULL, " "));
                rect.x = atoi(strtok(NULL, " "));
                rect.y = atoi(strtok(NULL, " "));
                rect.sx = atoi(strtok(NULL, " "));
                rect.sy = atoi(strtok(NULL, " "));

                vroi.push_back(rect);
            } while (rect.sx != 0 && rect.sy != 0);
            vroi.pop_back();
            utility::adjustBrightness(src, tgt, vroi);

        } else if (strcmp(pch, "smoothing") == 0) {
            /* Smoothing Filter */
            vector<w_roi> vroi;
            w_roi rect;
        
            do {
                rect.window = atoi(strtok(NULL, " "));
                rect.x = atoi(strtok(NULL, " "));
                rect.y = atoi(strtok(NULL, " "));
                rect.sx = atoi(strtok(NULL, " "));
                rect.sy = atoi(strtok(NULL, " "));

                vroi.push_back(rect);
            } while (rect.sx != 0 && rect.sy != 0);
            vroi.pop_back();
            utility::smoothing(src, tgt, vroi);

        } else if (strcmp(pch, "cbm") == 0) {
            cout << "cbm" << endl;
            /* Color image brightness */
            vector<cm_roi> vroi;
            cout << "inja" << endl;
            cm_roi rect;
            do {
                // rect.threshold = atoi(strtok(NULL, " "));
                cout << "inja too" << endl;
                rect.more_c = atoi(strtok(NULL, " "));
                rect.x = atoi(strtok(NULL, " "));
                rect.y = atoi(strtok(NULL, " "));
                rect.sx = atoi(strtok(NULL, " "));
                rect.sy = atoi(strtok(NULL, " "));

                vroi.push_back(rect);
            } while (rect.sx != 0 && rect.sy != 0);
            vroi.pop_back();
            utility::colorAdjustBrightness(src, tgt, vroi);

        } else if (strcmp(pch, "cb") == 0) {
            /* Color image brightness */
            vector<cb_roi> vroi;
            cb_roi rect;
            do {
                rect.tc = atoi(strtok(NULL, " "));
                rect.cr = atoi(strtok(NULL, " "));
                rect.cg = atoi(strtok(NULL, " "));
                rect.cb = atoi(strtok(NULL, " "));
                rect.x = atoi(strtok(NULL, " "));
                rect.y = atoi(strtok(NULL, " "));
                rect.sx = atoi(strtok(NULL, " "));
                rect.sy = atoi(strtok(NULL, " "));

                vroi.push_back(rect);
            } while (rect.sx != 0 && rect.sy != 0);
            vroi.pop_back();
            utility::colorBinarize(src, tgt, vroi);

        } else {
            printf("No function: %s\n", pch);
            continue;
        }

        tgt.save(outfile);
    }
    fclose(fp);
    return 0;
}

