*** INSTALATION ***

1) Install MinGW (http://sourceforge.net/projects/mingw/files/Installer/).
2) Install Make application for Windows (http://gnuwin32.sourceforge.net/packages/make.htm).
3) Install OpenCV.
3) Open command line and go to OpenCV directory.
4) Execute the command below to make the code:
	g++ iptool.cpp -o iptool -std=c++17 `pkg-config --cflags --libs opencv`
5) Run the code



*** FUNCTIONS ***

1. Sobel Edge Detection: sobel
This function use sobel algorithm to detects the edges of images.

2. Color Sobel Edge Detection: csobel
This function use sobel algorithm to detects the edges of color images.

3. Canny Edge Detection: canny
This function use canny algorithm to detects the edges of images.

4. Color Canny Edge Detection: ccanny
This function use canny algorithm to detects the edges of color images.

5. Histogram Equalization: equalization
Histogram equalization is a method in image processing of contrast adjustment using the image's histogram.

5. Color Histogram Equalization: cequalization
Histogram equalization is a method in image processing of contrast adjustment using the image's histogram in color images.

6. Extract Foreground: otsu
This function extract foreground of image by calculating Otsu threshold


*** PARAMETERS ***

1. Sobel Edge Detection: 
	input output sobel roi# x y sx sy window

2. Color Sobel Edge Detection:
	input output csobel roi# x y sx sy window

3. Canny Edge Detection:
	input output canny roi# x y sx sy window

4. Color Canny Edge Detection:
	input output ccanny roi# x y sx sy window

5. Histogram Equalization:
	input output qualization roi# x y sx sy

5. Color Histogram Equalization:
	input output cqualization roi# x y sx sy

6. Extract Foreground:
	input output otsu roi# x y sx sy



*** Run the program: iptool parameters.txt

