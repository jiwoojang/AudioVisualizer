#pragma once

#define _USE_MATH_DEFINES

#ifndef GLFW_DLL
	#define GLFW_DLL
#endif

#include "AudioVis.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "glm/gtc/matrix_transform.hpp"	// For matrix transformation functions
#include "glm/gtx/transform.hpp"	// For rotation matrix

using namespace std;
using namespace glm;

class Object3D
{
public:
	struct PackedVertex
	{
		vec3 pos;
		vec3 normal;
		vec2 uv;

		// To allow the use of this type as a key for a hashmap
		bool operator==(const Object3D::PackedVertex& other) const {
			return pos == other.pos && normal == other.normal && uv == other.uv;
		}
	};

	Object3D(string file, string texture);
	~Object3D();

	bool Init();
	void Draw();

private:

	bool LoadModel();
	void InitBuffers();
	bool InitShaders();
	bool InitTextures();

	// Files
	string filePath;
	string texturePath;

	// Buffer IDs
	GLuint vertexBuffer;
	GLuint uvBuffer;
	GLuint normalBuffer;
	GLuint elementBuffer;
	GLuint shader;
	GLuint texture;
	GLuint textureID;
	GLuint matrixID;

	// Object data
	vector<vec3> vertices;
	vector<vec2> uvs;
	vector<vec3> normals;
	vector<uint32_t> indices;
};