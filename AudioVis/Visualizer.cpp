#include "Visualizer.h"

Visualizer::Visualizer(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
}

Visualizer::~Visualizer()
{
	Teardown();
}

void Visualizer::Teardown()
{
	// Clean up for buffers
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(shaderId);

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

	// Order of operations here matters
	// Specifically the VAO initialization
	InitVAO();
	InitShaders();
	InitBuffers();
	
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
	window = glfwCreateWindow(windowWidth, windowHeight, "Playground", NULL, NULL);

	// Enable backface culling
	glEnable(GL_CULL_FACE);

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

	//// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	return true;
}

void Visualizer::InitVAO()
{
	// NOTE: It may look like this code gets used no where, but making the Vertext Array Object is important!
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
}

void Visualizer::InitBuffers()
{
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

void Visualizer::InitShaders()
{
	// Create and compile our GLSL program from the shaders
	shaderId = LoadShaders("Shaders/SimpleVertexShader.vertexshader", "Shaders/SimpleFragmentShader.fragmentshader");
}

void Visualizer::Update()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	// Use our shader
	glUseProgram(shaderId);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

	glDisableVertexAttribArray(0);

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}