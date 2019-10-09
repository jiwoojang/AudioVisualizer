# AudioVisualizer
An FFT based audio visualizer built with C++ and OpenGL

## Goals
This project was meant to be my first forray into graphics programming and also audio file processing.

The architecture is pretty short and sweet. One object holds and initializes audio data from a .wav file. I sample this file at regular intervals and perform a FFT on the samples. This gives me logarithmic frequency data to feed to the OpenGL rendere object. This object then manipulates the patters of the data and passes the final numbers onto the shader for rendering.'

If you would like to see the debug output for just the FFT, uncomment the `#define DEBUG_DRAW` in `AudioVis.h` to see an FFT of your .wav similar to the one below:

<p align="center">
  <img width="400" height="250" src="https://i.gyazo.com/6ddf6cf03bc4b7c850b2400d75759f9d.gif">
</p>

The final visualizer dynamically responds to an inputted .wav file, just like the gif below.

<p align="center">
  <img width="500" height="350" src="https://i.gyazo.com/9b3d8bbc1dea8684f7528eedeab57db0.gif">
</p>

## Building

The included Visual Studio solution file should be self contained and links to all the libraries and tools in the /External folder. Open it up in VS2019 and run in whatever configuration you please.

## Tools and libraries
* [SFML](https://www.sfml-dev.org/) was used for audio file reading and sampling
* A cheap FFT based on the Cooley-Turkey algorithm was implemented from [here](https://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B)
* Inspiration and boilerplate code for OpenGL was taken from my [own tutorial repo](https://github.com/jiwoojang/openGLPlayground)
* OpenGL tooling includes:
  * [GLEW](http://glew.sourceforge.net/) for extension management
  * [GLFW](https://www.glfw.org/) for easy window and context work
  * [GLM](https://glm.g-truc.net/0.9.9/index.html) for math classes and methods
