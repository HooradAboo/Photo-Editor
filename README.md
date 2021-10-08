
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


*** PARAMETERS ***

There are for parameters:
1. the input file name;
2. the output file name;
3. the name of the filter. Use "add", "binarize", "scale", "brightness", and "smoothing" for grayscale filters and use "cbinarize", and "cbrightness" for color filters;
4. the parmeters for all functions except "scale" function:
	orginal_img_adr modified_img_adr filter_name roi# inputs ...
5. the inputs for each fucntion except "scale" function. (you can add more than one inputs)
	add: value x y sx sy
	binarize: threshold x y sx sy
	brightness: threshold value1 value2 x y sx sy
	smoothing: window_size x y sx sy
	cbinarize: more-c x y sx sy
	cbrightness: tc cr cg cb x y sx sy
	stretching: x y sx sy a b
	tstretching: x y sx sy threshold a1 b1 a2 b2
	chstretching: x y sx sy channel a b
	cstretching: x y sx sy a b
	hstretching: x y sx sy a b
	hsstretching: x y sx sy a1 b1 a2 b2
	hsistretching: x y sx sy a1 b1 a2 b2 a3 b3
6. the parameters for "scale":
	orginal_img_adr modified_img_adr scale scaling_factor


*** Run the program: iptool.exe parameters.txt


*** Important information ***

Application assumes the next format of input image (ppm/pgm) file:
line1: <version>
line2: <#columns> <#rows>
line3: <max_value>
line4-end-of-file:<pix1><pix2>...<pix_n>

if it is a grayscale image then every pixel is a char value. If it is a RGB image then every pixel is a set of 3 char values: <red><green><blue>

Thus, if you have a problem with reading image, the first thing you should check is input file format.
