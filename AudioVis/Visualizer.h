#pragma once

#define _USE_MATH_DEFINES

#ifndef GLFW_DLL
	#define GLFW_DLL
#endif

#include "AudioVis.h"
#include "Object3D.h"

#include <stdio.h>
#include <chrono>	// For high_resolution_clock

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"

using namespace std;
using namespace glm;
using namespace chrono;

class AudioObject;

//==============================================================
// A wrapper class for all OpenGL related house keeping
//==============================================================
class Visualizer
{
public:

	Visualizer(int width, int height);
	~Visualizer();

	bool Init();
	void Update(const AudioObject& audioObject);

	const double& GetDeltaTime() const { return deltaTime; }
private:

	//--------------------------------------------------------------
	// Setup / Teardown
	//--------------------------------------------------------------
	bool InitWindow();
	void InitVAO();

	void Teardown();

	GLFWwindow* window;

	GLuint vertexArrayID;

	int windowWidth		{ 0 };
	int windowHeight	{ 0 };

	//--------------------------------------------------------------
	// Object to render
	//--------------------------------------------------------------
	Object3D object;

	//--------------------------------------------------------------
	// For deltaTime, always in seconds
	//--------------------------------------------------------------
	double						deltaTime { 0 };
	time_point<steady_clock>	lastTimeStamp;
};