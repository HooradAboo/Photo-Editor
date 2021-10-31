#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#define MAXLEN 256


struct roi {
    int x, y;
    int sx, sy;
};


Mat histogram (Mat src, char *input) 
{
    Mat tgt0, tgt1, tgt2;

    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3) ; r++)
    {
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;

        // create sub image
        Rect roi(rect.x, rect.y, rect.sx-rect.x, rect.sy-rect.y);        

        rectangle(src, roi, Scalar(255), 1, 8, 0);

        int histSize = 256;

        float range[] = { 0, 256 }; //the upper boundary is exclusive
        const float* histRange = { range };

        bool uniform = true, accumulate = false;

        int hist_w = 512, hist_h = 400;
        int bin_w = cvRound( (double) hist_w/histSize );

        if (r == 0) {
            Mat Roi0 = src(roi);

            vector<Mat> bgr_planes;
            split(Roi0, bgr_planes);

            Mat b_hist, g_hist, r_hist;
            calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
            calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
            calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

            Mat tgt0( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
            normalize( b_hist, b_hist, 0, tgt0.rows, NORM_MINMAX, -1, Mat() );
            normalize( g_hist, g_hist, 0, tgt0.rows, NORM_MINMAX, -1, Mat() );
            normalize( r_hist, r_hist, 0, tgt0.rows, NORM_MINMAX, -1, Mat() );

            for( int i = 1; i < histSize; i++ )
            {
                line( tgt0, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
                    Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                    Scalar( 255, 0, 0), 2, 8, 0  );
                line( tgt0, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
                    Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                    Scalar( 0, 255, 0), 2, 8, 0  );
                line( tgt0, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
                    Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                    Scalar( 0, 0, 255), 2, 8, 0  );
            }
            // imshow("First ROI", tgt0);
            imwrite("histogram_1stROI.ppm", tgt0);

        } else if (r == 1) {
            Mat Roi1 = src(roi);

            vector<Mat> bgr_planes;
            split(Roi1, bgr_planes);

            Mat b_hist, g_hist, r_hist;
            calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
            calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
            calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

            Mat tgt1( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
            normalize( b_hist, b_hist, 0, tgt1.rows, NORM_MINMAX, -1, Mat() );
            normalize( g_hist, g_hist, 0, tgt1.rows, NORM_MINMAX, -1, Mat() );
            normalize( r_hist, r_hist, 0, tgt1.rows, NORM_MINMAX, -1, Mat() );

            for( int i = 1; i < histSize; i++ )
            {
                line( tgt1, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
                    Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                    Scalar( 255, 0, 0), 2, 8, 0  );
                line( tgt1, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
                    Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                    Scalar( 0, 255, 0), 2, 8, 0  );
                line( tgt1, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
                    Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                    Scalar( 0, 0, 255), 2, 8, 0  );
            }
            // imshow("Second ROI", tgt1);
            imwrite("histogram_2edROI.ppm", tgt1);

        } else if (r == 2) { 
            Mat Roi2 = src(roi);

            vector<Mat> bgr_planes;
            split(Roi2, bgr_planes);

            Mat b_hist, g_hist, r_hist;
            calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
            calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
            calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

            Mat tgt2( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
            normalize( b_hist, b_hist, 0, tgt2.rows, NORM_MINMAX, -1, Mat() );
            normalize( g_hist, g_hist, 0, tgt2.rows, NORM_MINMAX, -1, Mat() );
            normalize( r_hist, r_hist, 0, tgt2.rows, NORM_MINMAX, -1, Mat() );

            for( int i = 1; i < histSize; i++ )
            {
                line( tgt2, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
                    Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                    Scalar( 255, 0, 0), 2, 8, 0  );
                line( tgt2, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
                    Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                    Scalar( 0, 255, 0), 2, 8, 0  );
                line( tgt2, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
                    Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                    Scalar( 0, 0, 255), 2, 8, 0  );
            }
            // imshow("Third ROI", tgt2);
            imwrite("histogram_3rdROI.ppm", tgt2);
        }
    }

    return src;
}


Mat sobelEdgeDetection (Mat src, char *input)
{
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3) ; r++)
    {
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse input window size
        int window = atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "W-Size: " << window << endl;

        // create sub image
        Rect roi(rect.x, rect.y, rect.sx-rect.x, rect.sy-rect.y);        

        rectangle(src, roi, Scalar(255), 1, 8, 0);
        // imshow ("Region of Interests", src);

        bool dx = 1, dy = 1;
        if (r == 0) {
            Mat tgt0;
            Mat Roi0 = src(roi);                            // sub image
            cvtColor(Roi0, Roi0, COLOR_BGR2GRAY);           // convert to gray scale
            Sobel(Roi0, tgt0, CV_64F, dx, dy, window);
            // imshow("First ROI", tgt0);
            imwrite("sobel_1stROI.pgm", tgt0);

        } else if (r == 1) {
            Mat tgt1;
            Mat Roi1 = src(roi);
            cvtColor(Roi1, Roi1, COLOR_BGR2GRAY);
            Sobel(Roi1, tgt1, CV_64F, dx, dy, window);
            // imshow("Second ROI", Roi1);
            imwrite("sobel_2edROI.pgm", tgt1);
        
        } else if (r == 2) { 
            Mat tgt2;
            Mat Roi2 = src(roi);
            cvtColor(Roi2, Roi2, COLOR_BGR2GRAY);
            Sobel(Roi2, tgt2, CV_64F, dx, dy, window);
            // imshow("Third ROI", tgt2);
            imwrite("sobel_3rdROI.pgm", tgt2);
        }
    }

    return src;
}


Mat cannyEdgeDetection (Mat src, char *input)
{
    Mat tgt0, tgt1, tgt2;

    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3) ; r++)
    {
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // parse input window size
        int window = atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "W-Size: " << window << endl;

        // create sub image
        Rect roi(rect.x, rect.y, rect.sx-rect.x, rect.sy-rect.y);        

        rectangle(src, roi, Scalar(255), 1, 8, 0);
        // imshow ("Region of Interests", src);

        bool dx = 1, dy = 1;
        if (r == 0) {
            Mat Roi0 = src(roi);                            // sub image
            cvtColor(Roi0, Roi0, COLOR_BGR2GRAY);           // convert to gray scale
            Canny(Roi0, tgt0, 100, 200, window, false);
            // imshow("First ROI", tgt0);
            imwrite("canny_1stROI.pgm", tgt0);
        } else if (r == 1) {
            Mat Roi1 = src(roi);
            cvtColor(Roi1, Roi1, COLOR_BGR2GRAY);
            Canny(Roi1, tgt1, 100, 200, window, false);
            // imshow("Second ROI", tgt1);
            imwrite("canny_2edROI.pgm", tgt1);
        } else if (r == 2) { 
            Mat Roi2 = src(roi);
            cvtColor(Roi2, Roi2, COLOR_BGR2GRAY);
            Canny(Roi2, tgt2, 100, 200, window, false);
            // imshow("Third ROI", tgt2);
            imwrite("canny_3rdROI.pgm", tgt2);
        }
    }

    return src;
}


Mat histogramEqualization (Mat src, char *input)
{
    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3) ; r++)
    {
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;

        // create sub image
        Rect roi(rect.x, rect.y, rect.sx-rect.x, rect.sy-rect.y);        

        rectangle(src, roi, Scalar(255), 1, 8, 0);
        // imshow ("Region of Interests", src);

        bool dx = 1, dy = 1;
        if (r == 0) {
            Mat tgt0;
            Mat Roi0 = src(roi); 

            cvtColor(Roi0, tgt0, COLOR_BGR2YCrCb);

            vector<Mat> vec_channels;
            split(tgt0, vec_channels); 

            equalizeHist(vec_channels[0], vec_channels[0]);
            merge(vec_channels, tgt0);    
            cvtColor(tgt0, tgt0, COLOR_YCrCb2BGR);

            tgt0.copyTo(src(roi));
            // imwrite("eq_1stROI.pgm", tgt0);

        } else if (r == 1) {
            Mat tgt1;
            Mat Roi1 = src(roi);  

            cvtColor(Roi1, tgt1, COLOR_BGR2YCrCb);

            vector<Mat> vec_channels;
            split(tgt1, vec_channels);

            equalizeHist(vec_channels[0], vec_channels[0]);
            merge(vec_channels, tgt1);    
            cvtColor(tgt1, tgt1, COLOR_YCrCb2BGR);

            tgt1.copyTo(src(roi));
            // imwrite("eq_2edROI.pgm", tgt1);

        } else if (r == 2) { 
            Mat tgt2;
            Mat Roi2 = src(roi); 

            cvtColor(Roi2, tgt2, COLOR_BGR2YCrCb);
                                    
            vector<Mat> vec_channels;
            split(tgt2, vec_channels); 

            equalizeHist(vec_channels[0], vec_channels[0]);
            merge(vec_channels, tgt2);    
            cvtColor(tgt2, tgt2, COLOR_YCrCb2BGR);

            tgt2.copyTo(src(roi));
            // imwrite("eq_3rdROI.pgm", tgt2);
        }
    }

    return src;
}

Mat otsuThresholding (Mat src, char *input) 
{
    Mat tgt0, tgt1, tgt2;

    // skip 3 first input (input image name, output image name, fuction name)
    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3) ; r++)
    {
        // parse input RoI
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;

        // create sub image
        Rect roi(rect.x, rect.y, rect.sx-rect.x, rect.sy-rect.y);        

        rectangle(src, roi, Scalar(255), 1, 8, 0);

        bool dx = 1, dy = 1;
        if (r == 0) {
            Mat Roi0 = src(roi);
            cvtColor(Roi0, Roi0, COLOR_BGR2GRAY); 
            long double thresh0 = cv::threshold(Roi0, tgt0, 170, 255, THRESH_OTSU);
            cout << "Otsu Threshold = " << thresh0 << endl;
            imwrite("otsu_1stROI.pgm", tgt0);

        } else if (r == 1) {
            Mat Roi1 = src(roi);
            cvtColor(Roi1, Roi1, COLOR_BGR2GRAY);
            long double thresh1 = cv::threshold(Roi1, tgt1, 0, 255, THRESH_OTSU);
            cout << "Otsu Threshold = " << thresh1 << endl;
            imwrite("otsu_2edROI.pgm", tgt1);

        } else if (r == 2) { 
            Mat Roi2 = src(roi);
            cvtColor(Roi2, Roi2, COLOR_BGR2GRAY);
            long double thresh2 = cv::threshold(Roi2, tgt2, 0, 255, THRESH_OTSU);
            cout << "Otsu Threshold = " << thresh2 << endl;
            imwrite("otsu_3rdROI.pgm", tgt2);
        }
    }

    return src;
}


int main(int argc, char **argv)
{   
    Mat src, tgt;
    FILE *fp;
    char str[MAXLEN];
    char input[MAXLEN];
    char outfile[MAXLEN];
    char *pch;
    char *function;
    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Can't open file: %s\n", argv[1]);
        exit(1);
    }


    while (fgets(str, MAXLEN, fp) != NULL) 
    {
        strcpy (input, str);

        pch = strtok(str, " ");
        src = imread (pch);

        pch = strtok(NULL, " ");
        strcpy(outfile, pch);

        function = strtok(NULL, " ");

        int roi_num = atoi(strtok(NULL, " "));  // number of RoI

        if (strcmp(function, "histogram") == 0) {
            /* Draw Histogram */
            tgt = histogram(src, input);
            cvtColor(tgt, tgt, COLOR_BGR2GRAY);

        } else if (strcmp(function, "chistogram") == 0) {
            /* Draw Histogram */
            tgt = histogram(src, input);

        }else if (strcmp(function, "sobel") == 0) {
            /* Sobel Edge Detection */
            tgt = sobelEdgeDetection(src, input);
            cvtColor(tgt, tgt, COLOR_BGR2GRAY);

        } else if (strcmp(function, "csobel") == 0) {
            /* Sobel Edge Detection */
            tgt = sobelEdgeDetection(src, input);

        } else if (strcmp(function, "canny") == 0) {
            /* Canny Edge Detection */
            tgt = cannyEdgeDetection(src, input);
            cvtColor(tgt, tgt, COLOR_BGR2GRAY);

        } else if (strcmp(function, "ccanny") == 0) {
            /* Canny Edge Detection */
            tgt = cannyEdgeDetection(src, input);

        } else if (strcmp(function, "equalization") == 0) {
            /* Histogram Equalization */
            tgt = histogramEqualization(src, input);
            cvtColor(tgt, tgt, COLOR_BGR2GRAY);

        } else if (strcmp(function, "cequalization") == 0) {
            /* Histogram Equalization */
            tgt = histogramEqualization(src, input);

        } else if (strcmp(function, "otsu") == 0) {
            /* Histogram Equalization */
            tgt = otsuThresholding(src, input);
            cvtColor(tgt, tgt, COLOR_BGR2GRAY);

        } else if (strcmp(function, "cotsu") == 0) {
            /* Histogram Equalization */
            tgt = otsuThresholding(src, input);

        } else {
            printf("No function: %s\n", pch);
            continue;
        } 
        
        // imshow(outfile, tgt);
        imwrite(outfile, tgt);
    }

    waitKey(0);
    destroyAllWindows();
}