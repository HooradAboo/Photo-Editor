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


int main(int argc, char **argv) {
    image src, tgt;
    FILE *fp;
    char str[MAXLEN], input[MAXLEN];
    char outfile[MAXLEN];
    char *pch;
    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Can't open file: %s\n", argv[1]);
        exit(1);
    }

    while (fgets(str, MAXLEN, fp) != NULL) {
        strcpy (input, str);
        pch = strtok(str, " ");
        src.read(pch);

        pch = strtok(NULL, " ");
        strcpy(outfile, pch);

        pch = strtok(NULL, " ");

        if (strcmp(pch, "add") == 0) {
            /* Add Intensity */
            vector<v_roi> vroi;
            v_roi rect;
            int roi_num = atoi(strtok(NULL, " "));
            for (int i = 0; i < roi_num; i++) {
                rect.value = atoi(strtok(NULL, " "));
                rect.x = atoi(strtok(NULL, " "));
                rect.y = atoi(strtok(NULL, " "));
                rect.sx = atoi(strtok(NULL, " "));
                rect.sy = atoi(strtok(NULL, " "));

                vroi.push_back(rect);
            }
            utility::addGrey(src, tgt, vroi);

        } else if (strcmp(pch, "binarize") == 0) {
            /* Thresholding */
            vector<t_roi> vroi;
            t_roi rect;
            int roi_num = atoi(strtok(NULL, " "));
            for (int i = 0; i < roi_num; i++) {
                rect.threshold = atoi(strtok(NULL, " "));
                rect.x = atoi(strtok(NULL, " "));
                rect.y = atoi(strtok(NULL, " "));
                rect.sx = atoi(strtok(NULL, " "));
                rect.sy = atoi(strtok(NULL, " "));

                vroi.push_back(rect);
            } 
            utility::binarize(src, tgt, vroi);

        } else if (strcmp(pch, "scale") == 0) {
            /* Image scaling */
            pch = strtok(NULL, " ");
            utility::scale(src, tgt, atof(pch));

        } else if (strcmp(pch, "brightness") == 0) {
            /* Image brightness */
            vector<vtv_roi> vroi;
            vtv_roi rect;
            int roi_num = atoi(strtok(NULL, " "));
            for (int i = 0; i < roi_num; i++) {
                rect.threshold = atoi(strtok(NULL, " "));
                rect.value1 = atoi(strtok(NULL, " "));
                rect.value2 = atoi(strtok(NULL, " "));
                rect.x = atoi(strtok(NULL, " "));
                rect.y = atoi(strtok(NULL, " "));
                rect.sx = atoi(strtok(NULL, " "));
                rect.sy = atoi(strtok(NULL, " "));

                vroi.push_back(rect);
            }
            utility::adjustBrightness(src, tgt, vroi);

        } else if (strcmp(pch, "smoothing") == 0) {
            /* Smoothing Filter */
            vector<w_roi> vroi;
            w_roi rect;
        
            int roi_num = atoi(strtok(NULL, " "));
            for (int i = 0; i < roi_num; i++) {
                rect.window = atoi(strtok(NULL, " "));
                rect.x = atoi(strtok(NULL, " "));
                rect.y = atoi(strtok(NULL, " "));
                rect.sx = atoi(strtok(NULL, " "));
                rect.sy = atoi(strtok(NULL, " "));

                vroi.push_back(rect);
            }
            utility::smoothing(src, tgt, vroi);

        } else if (strcmp(pch, "cbrightness") == 0) {
            /* Color image brightness */
            vector<cm_roi> vroi;
            cm_roi rect;
            int roi_num = atoi(strtok(NULL, " "));
            for (int i = 0; i < roi_num; i++) {
                rect.more_c = atoi(strtok(NULL, " "));
                rect.x = atoi(strtok(NULL, " "));
                rect.y = atoi(strtok(NULL, " "));
                rect.sx = atoi(strtok(NULL, " "));
                rect.sy = atoi(strtok(NULL, " "));

                vroi.push_back(rect);
            }
            utility::colorAdjustBrightness(src, tgt, vroi);

        } else if (strcmp(pch, "cbinarize") == 0) {
            /* Color image brightness */
            vector<cb_roi> vroi;
            cb_roi rect;
            int roi_num = atoi(strtok(NULL, " "));
            for (int i = 0; i < roi_num; i++) {
                rect.tc = atoi(strtok(NULL, " "));
                rect.cr = atoi(strtok(NULL, " "));
                rect.cg = atoi(strtok(NULL, " "));
                rect.cb = atoi(strtok(NULL, " "));
                rect.x = atoi(strtok(NULL, " "));
                rect.y = atoi(strtok(NULL, " "));
                rect.sx = atoi(strtok(NULL, " "));
                rect.sy = atoi(strtok(NULL, " "));

                vroi.push_back(rect);
            }
            utility::colorBinarize(src, tgt, vroi);

        } else if (strcmp(pch, "stretching") == 0) {
            utility::stretching(src, tgt, input);

        } else if (strcmp(pch, "cstretching") == 0) {
            utility::colorStretching(src, tgt, input);

        } else {
            printf("No function: %s\n", pch);
            continue;
        }

        tgt.save(outfile);
    }
    fclose(fp);
    return 0;
}


