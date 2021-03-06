
This software is architectured as follows

iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory.



*** INSTALATION ***

On Windows

1) Install MinGW (http://sourceforge.net/projects/mingw/files/Installer/).
2) Install Make application for Windows (http://gnuwin32.sourceforge.net/packages/make.htm).
3) Open command line and go to project directory.
4) Execite make.bat  

As a result you should get iptool.exe in project/bin directory.

NOTE: probably you will need to add location of MinGW files (gcc.exe, g++.exe and etc) and Make files (make.exe) to Path envirement variable. 

On Linux

1) Run make clean in iptool directory. (Or delete all .o in iptool directory)
2) Uncomment line 16 in project/MakeFile.
3) run make in project directory.

As a result you should get iptool in project/bin directory.


*** FUNCTIONS ***

1. Add intensity: add
Increase the intensity for a gray-level image.

2. Binarization: binarize
Binarize multiple region of intrest the pixels with the threshold.

3. Scaling: scale
Reduce or expand the heigh and width with twp scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.

4. Adjust Brightness: brightness
Increase the brightness of the pixel with the intensity larger than the threshold. 
Decrease the brightness of the pixel with the intensity smaller than the threshold.

5. Smoothing: smoothing
Filter the image with Average smoothing kernels of increasing standard deviations. The smoothing filter almost use for noise reduction.

6. Color Binarize: cbinarize
This function modify the intensity of each channel of each pixel based on the More-C.

7. Color Brightness: cbrightness
This function modify each pixel to red, green, or white based on the threshold.

8. Stretching: stretching
This function stretch histogram of gray-scale image and save input and output histogram.

9. Threshold Stretching: tstretching
This function classify pixels of gray-scale image to two goups of dark and bright pixel. Then, stretch histogram of each group and save input and output histogram.

10. Channel Stretching: chstretching
This function stretch histogram of color image (only the user provided channel) and save input and output histogram.

11. ColorStretching: cstretching
This function stretch histogram of all channels of color image and save input and output histogram.

12. Hue Stretching: hstretching
This function convert RGB to HSI. Then, stretch hue histogram. At the end, convert the output image from HSI to RGB and save input and output histogram.

13. Hue and Saturation Stretching: hsstretching
This function convert RGB to HSI. Then, stretch hue and saturation histogram. At the end, convert the output image from HSI to RGB and save input and output histogram.

14. Hue, Saturation and Intensity Stretching: hsistretching
This function convert RGB to HSI. Then, stretch hue, saturation and intnsity histogram. At the end, convert the output image from HSI to RGB and save input and output histogram.

15. Edge Detection: edge
This function points the edges of images using Sobel filter. You can use 2 and 5 window size.

16. Color Edge Detection: cedge
This function points the edges of  color images using Sobel filter. In this function RGB input convert to HSI and the detection applies on image intensity, then convert it back to RGB. You can use 2 and 5 window size.

17. Threshold Edge Detection: tedge
This function points the edges of images using Sobel filter and then, binarize the result based on user provided threshold. You can use 2 and 5 window size.

18. Direction Edge Detection: diredge
This function points the edges in horizental and 45 degree using Sobel filter and then, binarize the result based on user provided threshold. You can use 2 and 5 window size.


*** PARAMETERS ***

There are for parameters:
1. the input file name;
2. the output file name;
3. the name of the filter. Use "add", "binarize", "scale", "brightness", and "smoothing" for grayscale filters and use "cbinarize", and "cbrightness" for color filters;
4. the parmeters for all functions except "scale" function:
	orginal_img_adr modified_img_adr filter_name roi# inputs ...
5. the inputs for each fucntion except "scale" function. (you can add more than one inputs)
	add: value x y sx sy
	binarize: x y sx sy threshold
	brightness: threshold value1 value2 x y sx sy
	smoothing: window_size x y sx sy
	cbinarize: more-c x y sx sy
	cbrightness: tc cr cg cb x y sx sy
	
6. the parameters for "scale":
	orginal_img_adr modified_img_adr scale scaling_factor
	
7. the parameters for "Stretching" functions:
	Histgram:
		[input_img] [output_img] histogram #roi	x y sx sy
	Color Histgram:
		[input_img] [output_img] chistogram #roi x y sx sy
	Stretching:
		[input_img] [output_img] stretching #roi x y sx sy a b
	Threshold Stretching:
		[input_img] [output_img] tstretching #roi x y sx sy t a1 b1 a2 b2
	Channel Stretching:
		[input_img] [output_img] chstretching #roi x y sx sy ch a b
		* red = 0, green = 1, blue = 2
	Color Stretching:	
		[input_img] [output_img] cstretching #roi x y sx sy a b
	Intensity Stretching
		[input_img] [output_img] istretching #roi x y sx sy a b
		* (a,b) <= (0,100)
	

8. the parameters for "Edge Detection" functions:
	Edge Detection:
		[input_img] [output_img] edge roi# x y sx sy ws
	Threshold Edge Detection:
		[input_img] [output_img] tedge roi# x y sx sy ws th
	Direciton Edge Detection:
		[input_img] [output_img] edirdge roi# x y sx sy ws th
	Color Edge Detection:
		[input_img] [output_img] cedge roi# x y sx sy ws


*** Run the program: iptool.exe parameters.txt


*** Important information ***

Application assumes the next format of input image (ppm/pgm) file:
line1: <version>
line2: <#columns> <#rows>
line3: <max_value>
line4-end-of-file:<pix1><pix2>...<pix_n>

if it is a grayscale image then every pixel is a char value. If it is a RGB image then every pixel is a set of 3 char values: <red><green><blue>

Thus, if you have a problem with reading image, the first thing you should check is input file format.
