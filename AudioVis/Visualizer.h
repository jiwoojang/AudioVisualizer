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

class Visualizer
{
public:

	// Temp
	GLfloat g_vertex_buffer_data[9] 
	= {
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 0.0f,  1.0f, 0.0f,
	};

	Visualizer(int width, int height);
	~Visualizer();

	bool Init();
	void Update();
private:

	bool InitWindow();
	void InitVAO();

	void Teardown();

	GLFWwindow* window;

	GLuint vertexArrayID;

	int windowWidth		{ 0 };
	int windowHeight	{ 0 };

	Object3D object;
};