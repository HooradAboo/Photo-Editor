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
            utility::addGrey(src, tgt, input);

        } else if (strcmp(pch, "binarize") == 0) {
            /* Thresholding */
            utility::binarize(src, tgt, input);

        } else if (strcmp(pch, "scale") == 0) {
            /* Image scaling */
            pch = strtok(NULL, " ");
            utility::scale(src, tgt, atof(pch));

        } else if (strcmp(pch, "brightness") == 0) {
            /* Image brightness */
            utility::adjustBrightness(src, tgt, input);

        } else if (strcmp(pch, "smoothing") == 0) {
            /* Smoothing Filter */
            utility::smoothing(src, tgt, input);

        } else if (strcmp(pch, "cbrightness") == 0) {
            /* Color image brightness */
            utility::colorAdjustBrightness(src, tgt, input);

        } else if (strcmp(pch, "cbinarize") == 0) {
            /* Color image brightness */
            utility::colorBinarize(src, tgt, input);

        } else if (strcmp(pch, "histogram") == 0) {
            /* Image histogram stretching */
            utility::histogram(src, tgt, input);

        } else if (strcmp(pch, "chistogram") == 0) {
            /* Image histogram stretching */
            utility::colorHistogram(src, tgt, input);

        } else if (strcmp(pch, "stretching") == 0) {
            /* Image histogram stretching */
            utility::stretching(src, tgt, input);

        } else if (strcmp(pch, "tstretching") == 0) {
            /* Image histogram stretching with threshold*/
            utility::thresholdStretching(src, tgt, input);

        } else if (strcmp(pch, "chstretching") == 0) {
            /* Color image histogram stretching (single channel) */
            utility::channelStretching(src, tgt, input);

        } else if (strcmp(pch, "cstretching") == 0) {
            /* RGB image histogram stretching */
            utility::colorStretching(src, tgt, input);

        } else if (strcmp(pch, "hsi") == 0) {
            /* Convert RGB to HSI */
            utility::HSI(src, tgt);

        } else if (strcmp(pch, "rgb") == 0) {
            /* Convert HSI to RGB  */
            utility::RGB(src, tgt);

        } else if (strcmp(pch, "hstretching") == 0) {
            /* Hue stretching */
            utility::hueStretching(src, tgt, input);

        } else if (strcmp(pch, "sstretching") == 0) {
            /* Hue stretching */
            utility::saturationStretching(src, tgt, input);

        } else if (strcmp(pch, "istretching") == 0) {
            /* Hue stretching */
            utility::intensityStretching(src, tgt, input);

        } else if (strcmp(pch, "hsistretching") == 0) {
            /* HSI stretching */
            utility::HSIstretching(src, tgt, input);

        } else if (strcmp(pch, "edge") == 0) {
             /* Edge detection */
            utility::edgeDetection(src, tgt, input);

        } else if (strcmp(pch, "tedge") == 0) {
             /* Edge detection whith threshold */
            utility::thresholdEdgeDetection(src, tgt, input);

        } else if (strcmp(pch, "diredge") == 0) {
             /* Edge detection whith threshold */
            utility::directionEdgeDetection(src, tgt, input);

        } else if (strcmp(pch, "cedge") == 0) {
             /* Color edge detection */
            utility::colorEdgeDetection(src, tgt, input);

        } else {
            printf("No function: %s\n", pch);
            continue;
        }
        tgt.save(outfile);
        
    }
    fclose(fp);
    return 0;
}


