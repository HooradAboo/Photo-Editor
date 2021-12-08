#include <opencv2/opencv.hpp>
#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

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
    Mat tgt;
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

        // rectangle(src, roi, Scalar(255), 1, 8, 0);
        // imshow ("Region of Interests", src);

        // bool dx = 1, dy = 1;
        // if (r == 0) {
            Mat Roi0 = src(roi); 

            cvtColor(Roi0, tgt, COLOR_BGR2YCrCb);

            vector<Mat> vec_channels;
            split(tgt, vec_channels); 

            equalizeHist(vec_channels[0], vec_channels[0]);
            merge(vec_channels, tgt);    
            cvtColor(tgt, tgt, COLOR_YCrCb2BGR);

            // tgt.copyTo(src(roi));
            imshow("eq_1stROI.pgm", tgt);

        // } else if (r == 1) {
        //     Mat tgt1;
        //     Mat Roi1 = src(roi);  

        //     cvtColor(Roi1, tgt1, COLOR_BGR2YCrCb);

        //     vector<Mat> vec_channels;
        //     split(tgt1, vec_channels);

        //     equalizeHist(vec_channels[0], vec_channels[0]);
        //     merge(vec_channels, tgt1);    
        //     cvtColor(tgt1, tgt1, COLOR_YCrCb2BGR);

        //     tgt1.copyTo(src(roi));
        //     // imwrite("eq_2edROI.pgm", tgt1);

        // } else if (r == 2) { 
        //     Mat tgt2;
        //     Mat Roi2 = src(roi); 

        //     cvtColor(Roi2, tgt2, COLOR_BGR2YCrCb);
                                    
        //     vector<Mat> vec_channels;
        //     split(tgt2, vec_channels); 

        //     equalizeHist(vec_channels[0], vec_channels[0]);
        //     merge(vec_channels, tgt2);    
        //     cvtColor(tgt2, tgt2, COLOR_YCrCb2BGR);

        //     tgt2.copyTo(src(roi));
        //     // imwrite("eq_3rdROI.pgm", tgt2);
        // }
    }

    return tgt;
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


Mat fourier (Mat src, char *input)
{
    // src.convertTo(src, CV_8UC3, 255.0);
    cvtColor(src, src, COLOR_BGR2GRAY);


    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( src.rows );
    int n = getOptimalDFTSize( src.cols ); // on the border add zero values
    copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
    dft(complexI, complexI);            // this way the result may fit in the source matrix
    
    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(src))^2 + Im(DFT(src))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(src), planes[1] = Im(DFT(src))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat tgt = planes[0];
    tgt += Scalar::all(1);                    // switch to logarithmic scale
    log(tgt, tgt);
    
    // crop the spectrum, if it has an odd number of rows or columns    
    tgt = tgt(Rect(0, 0, tgt.cols & -2, tgt.rows & -2));
    
    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = tgt.cols/2;
    int cy = tgt.rows/2;
    
    Mat q0(tgt, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(tgt, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(tgt, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(tgt, Rect(cx, cy, cx, cy)); // Bottom-Right
    
    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(tgt, tgt, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).
    // imshow("Input Image"       , src   );    // Show the result
    tgt.convertTo(tgt, CV_8UC3, 255.0);
    imshow("spectrum magnitude", tgt);
    waitKey();

    return tgt;
}


void fftshift(const Mat &input_img, Mat &output_img)
{
	output_img = input_img.clone();
	int cx = output_img.cols / 2;
	int cy = output_img.rows / 2;
	Mat q1(output_img, Rect(0, 0, cx, cy));
	Mat q2(output_img, Rect(cx, 0, cx, cy));
	Mat q3(output_img, Rect(0, cy, cx, cy));
	Mat q4(output_img, Rect(cx, cy, cx, cy));

	Mat temp;
	q1.copyTo(temp);
	q4.copyTo(q1);
	temp.copyTo(q4);
	q2.copyTo(temp);
	q3.copyTo(q2);
	temp.copyTo(q3);
}


void calculateDFT(Mat &scr, Mat &dst)
{
	// define mat consists of two mat, one for real values and the other for complex values
	Mat planes[] = { scr, Mat::zeros(scr.size(), CV_32F) };
	Mat complexImg;
	merge(planes, 2, complexImg);

	dft(complexImg, complexImg);
	dst = complexImg;
}

Mat construct_H(Mat &scr, String type, float D0, float D1 = 250)
{
	Mat H(scr.size(), CV_32F, Scalar(1));
	float D = 0;
	if (type == "Low") {
        // cout << "Low Pass Filter" << endl;
        // cout << "Cut Off: " << D0 << endl;
		for (int u = 0; u < H.rows; u++)
		{
			for (int  v = 0; v < H.cols; v++)
			{
				D = sqrt((u - scr.rows / 2)*(u - scr.rows / 2) + (v - scr.cols / 2)*(v - scr.cols / 2));
				if (D > D0)
				{
					H.at<float>(u, v) = 0;
				}
			}
		}
	}
	else if (type == "High") {
        // cout << "High Pass Filter" << endl;
        // cout << "Cut Off: " << D0 << endl;
		for (int u = 0; u < H.rows; u++)
		{
			for (int  v = 0; v < H.cols; v++)
			{
				D = sqrt((u - scr.rows / 2)*(u - scr.rows / 2) + (v - scr.cols / 2)*(v - scr.cols / 2));
				if (D < D0)
				{
					H.at<float>(u, v) = 0;
				}
			}
		}
	} else if (type == "Band") {
        // cout << "Band Pass Filter" << endl;
        // cout << "Cut Off 1: " << D0 << endl;
        // cout << "Cut Off 2: " << D1 << endl;
		for (int u = 0; u < H.rows; u++)
		{
			for (int  v = 0; v < H.cols; v++)
			{
				D = sqrt((u - scr.rows / 2)*(u - scr.rows / 2) + (v - scr.cols / 2)*(v - scr.cols / 2));
				if ( D < D0 || D1 < D)
				{
					H.at<float>(u, v) = 0;
				}
			}
		}
	} else if (type == "Stop") {
        // cout << "Stop Pass Filter" << endl;
        // cout << "Cut Off 1: " << D0 << endl;
        // cout << "Cut Off 2: " << D1 << endl;
		for (int u = 0; u < H.rows; u++)
		{
			for (int  v = 0; v < H.cols; v++)
			{
				D = sqrt((u - scr.rows / 2)*(u - scr.rows / 2) + (v - scr.cols / 2)*(v - scr.cols / 2));
				if ( D0 < D && D < D1)
				{
					H.at<float>(u, v) = 0;
				}
			}
		}
	}
    return H;
}


void filtering(Mat &scr, Mat &dst, Mat &H)
{
	fftshift(H, H);
	Mat planesH[] = { Mat_<float>(H.clone()), Mat_<float>(H.clone()) };

	Mat planes_dft[] = { scr, Mat::zeros(scr.size(), CV_32F) };
	split(scr, planes_dft);

	Mat planes_out[] = { Mat::zeros(scr.size(), CV_32F), Mat::zeros(scr.size(), CV_32F) };
	planes_out[0] = planesH[0].mul(planes_dft[0]);
	planes_out[1] = planesH[1].mul(planes_dft[1]);

	merge(planes_out, 2, dst);

}


Mat lowPassFourier (Mat &src, string outfile, int D0)
{
    char *input;
    Mat tgt = fourier(src, input);
    int D = 0;
    
    for (int u = 0; u < tgt.rows; u++)
    {
        for (int  v = 0; v < tgt.cols; v++)
        {
            D = sqrt((u - tgt.rows / 2)*(u - tgt.rows / 2) + (v - tgt.cols / 2)*(v - tgt.cols / 2));
            if (D > D0)
            {
                tgt.at<uchar>(u, v) = 0;
            }
        }
    }

    imwrite(outfile, tgt);

    return tgt;
}



void lowPassFilter (Mat src, char *input)
{
    strtok(input, " ");
    char *outfile = strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    Mat roi;
    Mat DFT_image, H, complexIH;
    Mat tgt;


    for (int r = 0; r < min(roi_num, 3) ; r++)
    {
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));
        
        int cut_off = atoi(strtok(NULL, " "));
        
        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Cut Off: " << cut_off << endl;
        
        // create sub image
        Rect rec(rect.x, rect.y, rect.sx-rect.x, rect.sy-rect.y);        
        roi = src(rec);

        if (r == 0) {
            lowPassFourier (src, "low_roi1.pgm", cut_off);

            cvtColor(roi, roi, COLOR_BGR2GRAY);
            roi.convertTo(roi, CV_32F);

            calculateDFT(roi, DFT_image);

            H = construct_H(roi, "Low", cut_off);

            filtering(DFT_image, complexIH, H);

            dft(complexIH, tgt, DFT_INVERSE | DFT_REAL_OUTPUT);

            normalize(tgt, tgt, 0, 1, NORM_MINMAX);

            tgt.convertTo(tgt, CV_8UC3, 255.0);	

            imshow("Low Pass Filter 1", tgt);
            char str[80];
            strcpy (str, "1st_");
            strcat (str, outfile);
            imwrite (str, tgt);

        } else if (r == 1) {
            lowPassFourier (src, "low_roi2.pgm", cut_off);

            cvtColor(roi, roi, COLOR_BGR2GRAY);
            roi.convertTo(roi, CV_32F);

            calculateDFT(roi, DFT_image);

            H = construct_H(roi, "Low", cut_off);

            filtering(DFT_image, complexIH, H);

            dft(complexIH, tgt, DFT_INVERSE | DFT_REAL_OUTPUT);

            normalize(tgt, tgt, 0, 1, NORM_MINMAX);

            tgt.convertTo(tgt, CV_8UC3, 255.0);	

            imshow("Low Pass Filter 2", tgt);
            char str1[80];
            strcpy (str1, "2ed_");
            strcat (str1, outfile);
            imwrite (str1, tgt);

        } else if (r == 2) {

            lowPassFourier (src, "low_roi3.pgm", cut_off);

            cvtColor(roi, roi, COLOR_BGR2GRAY);
            roi.convertTo(roi, CV_32F);

            calculateDFT(roi, DFT_image);

            H = construct_H(roi, "Low", cut_off);

            filtering(DFT_image, complexIH, H);

            dft(complexIH, tgt, DFT_INVERSE | DFT_REAL_OUTPUT);

            normalize(tgt, tgt, 0, 1, NORM_MINMAX);

            tgt.convertTo(tgt, CV_8UC3, 255.0);	

            imshow("Low Pass Filter 3", tgt);
            char str2[80];
            strcpy (str2, "3rd_");
            strcat (str2, outfile);
            imwrite (str2, tgt);
        }
    }
    waitKey(0);
}


Mat highPassFourier (Mat &src, string outfile, int D0)
{
    char *input;
    Mat tgt = fourier(src, input);
    int D = 0;
    
    for (int u = 0; u < tgt.rows; u++)
    {
        for (int  v = 0; v < tgt.cols; v++)
        {
            D = sqrt((u - tgt.rows / 2)*(u - tgt.rows / 2) + (v - tgt.cols / 2)*(v - tgt.cols / 2));
            if (D < D0)
            {
                tgt.at<uchar>(u, v) = 0;
            }
        }
    }

    imwrite(outfile, tgt);

    return tgt;
}



void highPassFilter (Mat src, char *input)
{
    strtok(input, " ");
    char *outfile = strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    Mat roi;
    Mat DFT_image, H, complexIH;
    Mat tgt;


    for (int r = 0; r < min(roi_num, 3) ; r++)
    {
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));
        
        int cut_off = atoi(strtok(NULL, " "));
        
        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Cut Off: " << cut_off << endl;
        
        // create sub image
        Rect rec(rect.x, rect.y, rect.sx-rect.x, rect.sy-rect.y);        
        roi = src(rec);

        if (r == 0) {
            highPassFourier (src, "high_roi1.pgm", cut_off);

            cvtColor(roi, roi, COLOR_BGR2GRAY);
            roi.convertTo(roi, CV_32F);

            calculateDFT(roi, DFT_image);

            H = construct_H(roi, "High", cut_off);

            filtering(DFT_image, complexIH, H);

            dft(complexIH, tgt, DFT_INVERSE | DFT_REAL_OUTPUT);

            normalize(tgt, tgt, 0, 1, NORM_MINMAX);

            tgt.convertTo(tgt, CV_8UC3, 255.0);	

            imshow("High Pass Filter 1", tgt);
            char str[80];
            strcpy (str, "1st_");
            strcat (str, outfile);
            imwrite (str, tgt);

        } else if (r == 1) {

            highPassFourier (src, "high_roi2.pgm", cut_off);

            cvtColor(roi, roi, COLOR_BGR2GRAY);
            roi.convertTo(roi, CV_32F);

            calculateDFT(roi, DFT_image);

            H = construct_H(roi, "High", cut_off);

            filtering(DFT_image, complexIH, H);

            dft(complexIH, tgt, DFT_INVERSE | DFT_REAL_OUTPUT);

            normalize(tgt, tgt, 0, 1, NORM_MINMAX);

            tgt.convertTo(tgt, CV_8UC3, 255.0);	

            imshow("High Pass Filter 2", tgt);
            char str2[80];
            strcpy (str2, "2ed_");
            strcat (str2, outfile);
            imwrite (str2, tgt);

        } else if (r == 2) {

            highPassFourier (src, "high_roi3.pgm", cut_off);

            cvtColor(roi, roi, COLOR_BGR2GRAY);
            roi.convertTo(roi, CV_32F);

            calculateDFT(roi, DFT_image);

            H = construct_H(roi, "High", cut_off);

            filtering(DFT_image, complexIH, H);

            dft(complexIH, tgt, DFT_INVERSE | DFT_REAL_OUTPUT);

            normalize(tgt, tgt, 0, 1, NORM_MINMAX);

            tgt.convertTo(tgt, CV_8UC3, 255.0);	

            imshow("High Pass Filter 3", tgt);
            char str3[80];
            strcpy (str3, "3rd_");
            strcat (str3, outfile);
            imwrite (str3, tgt);
        }
    }
    waitKey(0);
}



Mat bandPassFourier (Mat &src, string outfile, int D0, int D1)
{
    char *input;
    Mat tgt = fourier(src, input);
    int D = 0;
    
    for (int u = 0; u < tgt.rows; u++)
    {
        for (int  v = 0; v < tgt.cols; v++)
        {
            D = sqrt((u - tgt.rows / 2)*(u - tgt.rows / 2) + (v - tgt.cols / 2)*(v - tgt.cols / 2));
            if (D < D0 || D1 < D)
            {
                tgt.at<uchar>(u, v) = 0;
            }
        }
    }

    imwrite(outfile, tgt);

    return tgt;
}



void bandPassFilter (Mat src, char *input)
{
    strtok(input, " ");
    char *outfile = strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    Mat roi;
    Mat DFT_image, H, complexIH;
    Mat tgt;


    for (int r = 0; r < min(roi_num, 3) ; r++)
    {
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));
        
        int cut_off1 = atoi(strtok(NULL, " "));
        int cut_off2 = atoi(strtok(NULL, " "));
        
        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Cut Off 1: " << cut_off1 << endl;
        cout << "Cut Off 2: " << cut_off2 << endl;
        
        // create sub image
        Rect rec(rect.x, rect.y, rect.sx-rect.x, rect.sy-rect.y);        
        roi = src(rec);

        if (r == 0) {
            bandPassFourier (src, "band_roi1.pgm", cut_off1, cut_off2);

            cvtColor(roi, roi, COLOR_BGR2GRAY);
            roi.convertTo(roi, CV_32F);

            calculateDFT(roi, DFT_image);

            H = construct_H(roi, "Band", cut_off1, cut_off2);

            filtering(DFT_image, complexIH, H);

            dft(complexIH, tgt, DFT_INVERSE | DFT_REAL_OUTPUT);

            normalize(tgt, tgt, 0, 1, NORM_MINMAX);

            tgt.convertTo(tgt, CV_8UC3, 255.0);	

            imshow("Band Pass Filter 1", tgt);
            char str[80];
            strcpy (str, "1st_");
            strcat (str, outfile);
            imwrite (str, tgt);

        } else if (r == 1) {

            bandPassFourier (src, "band_roi2.pgm", cut_off1, cut_off2);

            cvtColor(roi, roi, COLOR_BGR2GRAY);
            roi.convertTo(roi, CV_32F);

            calculateDFT(roi, DFT_image);

            H = construct_H(roi, "Band", cut_off1, cut_off2);

            filtering(DFT_image, complexIH, H);

            dft(complexIH, tgt, DFT_INVERSE | DFT_REAL_OUTPUT);

            normalize(tgt, tgt, 0, 1, NORM_MINMAX);

            tgt.convertTo(tgt, CV_8UC3, 255.0);	

            imshow("Band Pass Filter 2", tgt);
            char str2[80];
            strcpy (str2, "2ed_");
            strcat (str2, outfile);
            imwrite (str2, tgt);

        } else if (r == 2) {

            bandPassFourier (src, "band_roi3.pgm", cut_off1, cut_off2);

            cvtColor(roi, roi, COLOR_BGR2GRAY);
            roi.convertTo(roi, CV_32F);

            calculateDFT(roi, DFT_image);

            H = construct_H(roi, "Band", cut_off1, cut_off2);

            filtering(DFT_image, complexIH, H);

            dft(complexIH, tgt, DFT_INVERSE | DFT_REAL_OUTPUT);

            normalize(tgt, tgt, 0, 1, NORM_MINMAX);

            tgt.convertTo(tgt, CV_8UC3, 255.0);	

            imshow("Band Pass Filter 3", tgt);
            char str3[80];
            strcpy (str3, "3rd_");
            strcat (str3, outfile);
            imwrite (str3, tgt);
        }
    }
    waitKey(0);
}



Mat stopPassFourier (Mat &src, string outfile, int D0, int D1)
{
    char *input;
    Mat tgt = fourier(src, input);
    int D = 0;
    
    for (int u = 0; u < tgt.rows; u++)
    {
        for (int  v = 0; v < tgt.cols; v++)
        {
            D = sqrt((u - tgt.rows / 2)*(u - tgt.rows / 2) + (v - tgt.cols / 2)*(v - tgt.cols / 2));
            if (D > D0 && D1 > D)
            {
                tgt.at<uchar>(u, v) = 0;
            }
        }
    }

    imwrite(outfile, tgt);

    return tgt;
}



void stopPassFilter (Mat src, char *input)
{
    strtok(input, " ");
    char *outfile = strtok(NULL, " ");
    strtok(NULL, " ");

    roi rect;                               // Region of Intrest
    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    Mat roi;
    Mat DFT_image, H, complexIH;
    Mat tgt;


    for (int r = 0; r < min(roi_num, 3) ; r++)
    {
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));
        
        int cut_off1 = atoi(strtok(NULL, " "));
        int cut_off2 = atoi(strtok(NULL, " "));
        
        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Cut Off 1: " << cut_off1 << endl;
        cout << "Cut Off 2: " << cut_off2 << endl;
        
        // create sub image
        Rect rec(rect.x, rect.y, rect.sx-rect.x, rect.sy-rect.y);        
        roi = src(rec);

        if (r == 0) {
            stopPassFourier (src, "notch_roi1.pgm", cut_off1, cut_off2);

            cvtColor(roi, roi, COLOR_BGR2GRAY);
            roi.convertTo(roi, CV_32F);

            calculateDFT(roi, DFT_image);

            H = construct_H(roi, "Stop", cut_off1, cut_off2);

            filtering(DFT_image, complexIH, H);

            dft(complexIH, tgt, DFT_INVERSE | DFT_REAL_OUTPUT);

            normalize(tgt, tgt, 0, 1, NORM_MINMAX);

            tgt.convertTo(tgt, CV_8UC3, 255.0);	

            imshow("Notch Filter 1", tgt);
            char str[80];
            strcpy (str, "1st_");
            strcat (str, outfile);
            imwrite (str, tgt);

        } else if (r == 1) {

            stopPassFourier (src, "notch_roi2.pgm", cut_off1, cut_off2);

            cvtColor(roi, roi, COLOR_BGR2GRAY);
            roi.convertTo(roi, CV_32F);

            calculateDFT(roi, DFT_image);

            H = construct_H(roi, "Stop", cut_off1, cut_off2);

            filtering(DFT_image, complexIH, H);

            dft(complexIH, tgt, DFT_INVERSE | DFT_REAL_OUTPUT);

            normalize(tgt, tgt, 0, 1, NORM_MINMAX);

            tgt.convertTo(tgt, CV_8UC3, 255.0);	

            imshow("Notch Filter 2", tgt);
            char str2[80];
            strcpy (str2, "2ed_");
            strcat (str2, outfile);
            imwrite (str2, tgt);

        } else if (r == 2) {

            stopPassFourier (src, "notch_roi3.pgm", cut_off1, cut_off2);

            cvtColor(roi, roi, COLOR_BGR2GRAY);
            roi.convertTo(roi, CV_32F);

            calculateDFT(roi, DFT_image);

            H = construct_H(roi, "Stop", cut_off1, cut_off2);

            filtering(DFT_image, complexIH, H);

            dft(complexIH, tgt, DFT_INVERSE | DFT_REAL_OUTPUT);

            normalize(tgt, tgt, 0, 1, NORM_MINMAX);

            tgt.convertTo(tgt, CV_8UC3, 255.0);	

            imshow("Notch Filter 3", tgt);
            char str3[80];
            strcpy (str3, "3rd_");
            strcat (str3, outfile);
            imwrite (str3, tgt);
        }
    }
    waitKey(0);
}


Mat unsharpMasking(Mat src, char *input)
{
    cvtColor(src, src, COLOR_BGR2GRAY);

    roi rect;                               // Region of Intrest

    strtok(input, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    int roi_num = atoi(strtok(NULL, " "));  // number of RoI

    for (int r = 0; r < min(roi_num, 3) ; r++)
    {
        rect.x = atoi(strtok(NULL, " "));
        rect.y = atoi(strtok(NULL, " "));
        rect.sx = rect.x + atoi(strtok(NULL, " "));
        rect.sy = rect.y + atoi(strtok(NULL, " "));

        int amount = atoi(strtok(NULL, " "));

        // print input date
        cout << "(X,Y): (" << rect.x << "," << rect.y << ")" << endl;
        cout << "(X',Y'): (" << rect.sx << "," << rect.sy << ")" << endl;
        cout << "Multiplication: " << amount << endl;
        
        // create sub image
        Rect rec(rect.x, rect.y, rect.sx-rect.x, rect.sy-rect.y);        

        // rectangle(src, rec, Scalar(255), 1, 8, 0);

        Mat roi = src(rec);
        
        Mat blurry;
        GaussianBlur(roi, blurry, Size(), 1);
        Mat sharp = roi * (1 + amount) + blurry * (-amount);

        sharp.copyTo(src(rec));
    }

	imshow ("Shrped", src);   
    waitKey(0);

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
            imwrite(outfile, tgt);

        } else if (strcmp(function, "cequalization") == 0) {
            /* Histogram Equalization */
            tgt = histogramEqualization(src, input);
            imwrite(outfile, tgt);

        } else if (strcmp(function, "otsu") == 0) {
            /* Histogram Equalization */
            tgt = otsuThresholding(src, input);
            cvtColor(tgt, tgt, COLOR_BGR2GRAY);
            imwrite(outfile, tgt);

        } else if (strcmp(function, "cotsu") == 0) {
            /* Histogram Equalization */
            tgt = otsuThresholding(src, input);
            imwrite(outfile, tgt);

        } else if (strcmp(function, "fourier") == 0) {
            /* Fourier Transform */
            tgt = fourier(src, input);
            imwrite(outfile, tgt);

        } else if (strcmp(function, "lowpass") == 0) {
            /* Low Pass Filter */
            lowPassFilter(src, input);

        } else if (strcmp(function, "highpass") == 0) {
            /* High Pass Filter */
            highPassFilter(src, input);

        } else if (strcmp(function, "bandpass") == 0) {
            /* High Pass Filter */
            bandPassFilter(src, input);

        } else if (strcmp(function, "notch") == 0) {
            /* High Pass Filter */
            stopPassFilter(src, input);

        } else if (strcmp(function, "usm") == 0) {
            /* High Pass Filter */
            tgt = unsharpMasking(src, input);
            cout << tgt;
            imwrite(outfile, tgt);

        } else {
            printf("No function: %s\n", pch);
            continue;
        } 
        
        // imshow(outfile, tgt);
        // tgt.convertTo(tgt, CV_8UC3, 255.0);	
        // imwrite(outfile, tgt);
    }

    waitKey(0);
    destroyAllWindows();
}