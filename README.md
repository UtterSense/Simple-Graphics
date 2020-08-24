
## Table of Contents

* [Background](#background)
* [Installation](#installation)
* [Usage](#usage) 
* [Examples](#examples)

## Background
	The purpose of this project was to repackage some legacy software that had been written	for the
	Borland	Turbo C graphics library running on Windows systems. Graphics cpability was required for
	running simple graphics routines on the Raspberry Pi Linux platform. The graphics package had
	originally been	written in C++. However, for the purposes of this project, the coding has reverted
	back to C. Although rather basic,the Linux Graphics library provides all the necessary functionality
	required to generate scientific static plots and animated plot routines (for instance, displaying
	a real time electronic signal from an external electronic peripheral). The graphic_lx.c library is
	an abstraction of the Graphics.c library for generating scientific plots.
	
## Installation
   a good guide to installing the Linux Graphics Library may be found here:
   https://www.codingalpha.com/install-run-graphics-h-ubuntu/ 		
	
## Usage
   The graphic_lx.c library contains a mix of functions for setting up graphs with features such
   as title, axis scales and labelling, and plot grid. Various plot types can be realised: 
   line plot, pixel plot, circle or rectangle plots. A legend can be set up to accompany the
   main graph. Axis scales can be linear or logarithmic. 
   Graphic animations of varying signals in real time also be generated. A suitable graphic
   presentation can be developed by utilisng suitable functions within the library.   
	
## Examples
### Frequency Response curve   
   The following two graphs illustrate a frequency response curve for a high pass filter (HPF) sub-circuit
   on a breadboard connected to an Arduino Uno. Square waves of various frequencies are generated on one of the
   Arduino digital output pins and passed through the HPF.The Arduino MCU sends sampled data via UART to a Linux PC. 
   Frequencies are plotted logarithmically, and a graph plotted automatically on receipt of data through
   the PC serial port. The actual measured data is plotted with circles, and the theoretical curve
   (for the R/C HPF combination) plotted as a line. Graph 1 uses a black background, and Graph 2 uses a white background.
   
#### Graph 1
![Graph 1](images/freq_response_black.png)   	

#### Graph 2
![Graph 2](images/freq_response_white.png)

### Simple Oscilloscope Trace  
   Graph 3 illustrates the display of a signal generated in real time from a signal generator,passing
   through a high pass filter, and sent by an Arduino Uno via UART to a Raspberry Pi. The display connected to 
   the Raspberry Pi shows this graph varying in real time. The display is useful as an additional oscilloscope
   tool for analysing various signals on prototype development boards. 
      
#### Graph 3
![Graph 3](images/trace.png)   	
