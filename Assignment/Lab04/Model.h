#pragma once

#define _USE_MATH_DEFINES

// Windows includes
#include <vector>
#include <iostream>

// OpenGL includes
#include <GL/glew.h>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"
#include "stb_image.h"

typedef struct
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;

class Model
{
public:
	Model(const char* mesh_name, vec3 starting_pos);
	GLuint vao;
	vec3 pos, vel, rot, rot_vel, scale;
	bool isMoving, isTurningLeft, isTurningRight;
	Model::ModelData model_data;
	void update();
private:
	ModelData load_mesh(const char* file_name);
};