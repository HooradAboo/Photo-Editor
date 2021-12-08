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

7. Fourier Transform: fourier
This function transform the image to the frequency domain.

8. Low Pass Filter: lowpass
This function smooth the image using the low pass filter.

9. High Pass Filter: highpass
This function is kind of edge detection. In this method we use High pass filter to identify the edges.

10. Band Pass Filter: bandpass
This filter is a good method to remove the noise in the frequency domain is known.

11. Stop Pass Filter: notch
This functio is a filter that attenuates frequencies within a specific range while passing all other frequencies unaltered.

12. Unsharp Mask: usm
This function add edge sharpening capability via “unsharp masking” by increasing high-frequency components via multiplication by user provided constant parameter. 



*** PARAMETERS ***

1. Sobel Edge Detection: 
	[input] [output] sobel roi# x y sx sy window

2. Color Sobel Edge Detection:
	[input] [output] csobel roi# x y sx sy window

3. Canny Edge Detection:
	[input] [output] canny roi# x y sx sy window

4. Color Canny Edge Detection:
	[input] [output] ccanny roi# x y sx sy window

5. Histogram Equalization:
	[input] [output] qualization roi# x y sx sy

5. Color Histogram Equalization:
	[input] [output] cqualization roi# x y sx sy

6. Extract Foreground:
	[input] [output] otsu roi# x y sx sy

7. Fourier Transform:
	[input] [output] fourier roi# x y sx sy

8. Low Pass Filter:
	[input] [output] lowpass roi# x y sx sy cut_off

9. High Pass Filter:
	[input] [output] highpass roi# x y sx sy cut_off

10. Band Pass Filter:
	[input] [output] bandpass roi# x y sx sy cut_off1 cut_off2

11. Stop Pass Filter (Notch):
	[input] [output] notch roi# x y sx sy cut_off1 cut_off2

12. Unsharp Mask:
	[input] [output] usm roi# x y sx sy coefficient


* NOTE: The second parameter of all filters (output) is not important. These functions have few outputs with default names.



*** Run the program: iptool parameters.txt

