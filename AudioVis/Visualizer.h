#pragma once

#define _USE_MATH_DEFINES
#define GLFW_DLL
#include "AudioVis.h"
#include "Shader.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <chrono>	// For high_resolution_clock

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>	// For matrix transformation functions
#include <glm/gtx/transform.hpp>	// For rotation matrix

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
	void InitBuffers();
	void InitShaders();

	void Teardown();

	GLFWwindow* window;

	GLuint vertexBuffer;
	GLuint vertexArrayID;
	GLuint shaderId;

	int windowWidth		{ 0 };
	int windowHeight	{ 0 };
};