#include "Visualizer.h"

// TEST 
#include "Shader.hpp"

Visualizer::Visualizer(int width, int height)
	: object("Models/suzanne.obj", "Models/suzanneuvmap.DDS")
	,windowWidth(width)
	, windowHeight(height)
{
}

Visualizer::~Visualizer()
{
	Teardown();
}

void Visualizer::Teardown()
{
	// Clean up for buffers
	glDeleteVertexArrays(1, &vertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

bool Visualizer::Init()
{
	if (!InitWindow())
	{
		cout << "Window Initialization failed" << endl;
		return false;
	}

	InitVAO();

	if (!object.Init())
	{
		cout << "Object initialization failed" << endl;
		return false;
	}

	return true;
}

bool Visualizer::InitWindow()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		cout << "Failed to initialize GLDFW" << endl;
		Teardown();
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(windowWidth, windowHeight, "AudioVis", NULL, NULL);

	// Enable backface culling
	//glEnable(GL_CULL_FACE);

	if (window == NULL) {
		cout << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible, try GLFW 2.1" << endl;
		Teardown();
		return false;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK) {
		cout << "Failed to initialize GLEW\n" << endl;
		Teardown();
		return false;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	//// Enable blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	return true;
}

void Visualizer::InitVAO()
{
	// NOTE: It may look like this code gets used no where, but making the Vertext Array Object is important!
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
}


void Visualizer::Update()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	object.Draw();

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}
