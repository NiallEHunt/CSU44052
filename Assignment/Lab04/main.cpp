#pragma region INCLUDES AND DEFINES
#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Project includes
#include "maths_funcs.h"
#include "Model.h"
#include "Wheel.h"
#include "Camera.h"

#define X 0
#define Y 1
#define Z 2

#define NUMBER_OF_TREES 20
#pragma endregion INCLUDES AND DEFINES

// Model names to be loaded
#define CAR_BODY_MESH_NAME "models/car_body.obj"
#define CAR_TRIM_AND_WINDOWS_MESH_NAME "models/car_trim_and_windows.obj"
#define CAR_LIGHTS_MESH_NAME "models/car_lights.obj"
#define CAR_WHEEL_RIM_MESH_NAME "models/wheel_rim.obj"
#define CAR_WHEEL_TYRE_MESH_NAME "models/wheel_tyre.obj"
#define ROAD_MESH_NAME "models/road.obj"
#define TREE_LEAVES_MESH_NAME "models/tree_leaves.obj"
#define TREE_TRUNK_MESH_NAME "models/tree_trunk.obj"
#define SKYBOX_MESH_NAME "models/skybox.obj"
#define GROUND_MESH_NAME "models/ground.obj"

#pragma region TEXTURE NAMES
#define ROAD_TEXTURE_FILENAME "textures/road_texture.jpg"
#define ROAD_TEXTURE_NAME "road_texture"
#define SKYBOX_TEXTURE_FILENAME "textures/sky.png"
#define SKYBOX_TEXTURE_NAME "sky_texture"
#define GROUND_TEXTURE_FILENAME "textures/grass.JPG"
#define GROUND_TEXTURE_NAME "ground_texture"
#define RED_TEXTURE_FILENAME "textures/red.png"
#define RED_TEXTURE_NAME "red_texture"
#define BLACK_TEXTURE_FILENAME "textures/black.png"
#define BLACK_TEXTURE_NAME "black_texture"
#define WHITE_TEXTURE_FILENAME "textures/white.png"
#define WHITE_TEXTURE_NAME "white_texture"
#define GREY_TEXTURE_FILENAME "textures/grey.png"
#define GREY_TEXTURE_NAME "grey_texture"
#define BROWN_TEXTURE_FILENAME "textures/brown.png"
#define BROWN_TEXTURE_NAME "brown_texture"
#define GREEN_TEXTURE_FILENAME "textures/green.png"
#define GREEN_TEXTURE_NAME "green_texture"

#define ROAD_TEXTURE 0
#define SKYBOX_TEXTURE 1
#define GROUND_TEXTURE 2
#define RED_TEXTURE 3
#define BLACK_TEXTURE 4
#define WHITE_TEXTURE 5
#define GREY_TEXTURE 6
#define BROWN_TEXTURE 7
#define GREEN_TEXTURE 8
#pragma endregion TEXTURE NAMES

using namespace std;
GLuint shaderProgramID;

int width = 1600;
int height = 900;

GLuint loc1, loc2, loc3;
bool cam_lock = true;

Model road(ROAD_MESH_NAME, vec3(0.0f, 0.0f, 0.0f));
Model sky(SKYBOX_MESH_NAME, vec3(0.0f, 0.0f, 0.0f));
Model ground(GROUND_MESH_NAME, vec3(-100.0f, -0.1f, -100.0f));
Model tree_leaves(TREE_LEAVES_MESH_NAME, vec3(8.0f, 0.0f, -100.0f));
Model tree_trunk(TREE_TRUNK_MESH_NAME, vec3(8.0f, 0.0f, -100.0f));

// Car and Wheels
Model car_body(CAR_BODY_MESH_NAME, vec3(2.5f, 0.0f, 0.0f));
Model car_trim_and_windows(CAR_TRIM_AND_WINDOWS_MESH_NAME, vec3(0.0f, 0.0f, 0.0f));
Model car_lights(CAR_LIGHTS_MESH_NAME, vec3(0.0f, 0.0f, 0.0f));
Wheel fl_wheel_rim(CAR_WHEEL_RIM_MESH_NAME, vec3(0.86f, 0.4f, 1.3f));
Wheel fr_wheel_rim(CAR_WHEEL_RIM_MESH_NAME, vec3(-0.86f, 0.4f, 1.3f));
Wheel bl_wheel_rim(CAR_WHEEL_RIM_MESH_NAME, vec3(0.86f, 0.4f, -1.3f));
Wheel br_wheel_rim(CAR_WHEEL_RIM_MESH_NAME, vec3(-0.86f, 0.4f, -1.3f));
Wheel fl_wheel_tyre(CAR_WHEEL_TYRE_MESH_NAME, vec3(0.86f, 0.4f, 1.3f));
Wheel fr_wheel_tyre(CAR_WHEEL_TYRE_MESH_NAME, vec3(-0.86f, 0.4f, 1.3f));
Wheel bl_wheel_tyre(CAR_WHEEL_TYRE_MESH_NAME, vec3(0.86f, 0.4f, -1.3f));
Wheel br_wheel_tyre(CAR_WHEEL_TYRE_MESH_NAME, vec3(-0.86f, 0.4f, -1.3f));

// AI Car and Wheels
Model ai_car_body(CAR_BODY_MESH_NAME, vec3(-2.5f, 0.0f, 60.0f), true);
Model ai_car_trim_and_windows(CAR_TRIM_AND_WINDOWS_MESH_NAME, vec3(0.0f, 0.0f, 0.0f));
Model ai_car_lights(CAR_LIGHTS_MESH_NAME, vec3(0.0f, 0.0f, 0.0f));
Wheel ai_fl_wheel_rim(CAR_WHEEL_RIM_MESH_NAME, vec3(0.86f, 0.4f, 1.3f));
Wheel ai_fr_wheel_rim(CAR_WHEEL_RIM_MESH_NAME, vec3(-0.86f, 0.4f, 1.3f));
Wheel ai_bl_wheel_rim(CAR_WHEEL_RIM_MESH_NAME, vec3(0.86f, 0.4f, -1.3f));
Wheel ai_br_wheel_rim(CAR_WHEEL_RIM_MESH_NAME, vec3(-0.86f, 0.4f, -1.3f));
Wheel ai_fl_wheel_tyre(CAR_WHEEL_TYRE_MESH_NAME, vec3(0.86f, 0.4f, 1.3f));
Wheel ai_fr_wheel_tyre(CAR_WHEEL_TYRE_MESH_NAME, vec3(-0.86f, 0.4f, 1.3f));
Wheel ai_bl_wheel_tyre(CAR_WHEEL_TYRE_MESH_NAME, vec3(0.86f, 0.4f, -1.3f));
Wheel ai_br_wheel_tyre(CAR_WHEEL_TYRE_MESH_NAME, vec3(-0.86f, 0.4f, -1.3f));

Camera camera(vec3(0.0f, -2.5f, -10.0f));

GLuint textures[9];

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS
char* readShaderSource(const char* shaderFile) {
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		std::cerr << "Error creating shader..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	const char* pShaderSource = readShaderSource(pShaderText);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024] = { '\0' };
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cerr << "Error compiling "
			<< (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader program: " << InfoLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders()
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		std::cerr << "Error creating shader program..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	AddShader(shaderProgramID, "shaders/simpleVertexShader.txt", GL_VERTEX_SHADER);
	AddShader(shaderProgramID, "shaders/simpleFragmentShader.txt", GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { '\0' };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	glUseProgram(shaderProgramID);
	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS

// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS
void generateObjectBufferMesh(Model model) {
	ModelData mesh_data = model.model_data;
	unsigned int vp_vbo = 0;
	loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");

	glGenBuffers(1, &vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[X], GL_STATIC_DRAW);

	unsigned int vn_vbo = 0;
	glGenBuffers(1, &vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[X], GL_STATIC_DRAW);

	//	This is for texture coordinates which you don't currently need, so I have commented it out
	unsigned int vt_vbo = 0;
	if (mesh_data.mTextureCoords.size() > 0)
	{
		glGenBuffers(1, &vt_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vt_vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec2), &model.model_data.mTextureCoords[X], GL_STATIC_DRAW);
	}

	glBindVertexArray(model.vao);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//	This is for texture coordinates which you don't currently need, so I have commented it out
	if (mesh_data.mTextureCoords.size() > 0)
	{
		glEnableVertexAttribArray(loc3);
		glBindBuffer(GL_ARRAY_BUFFER, vt_vbo);
		glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	}
}
#pragma endregion VBO_FUNCTIONS

#pragma region TEXTURE_FUNCTIONS

void loadTexture(int active_texture, GLuint texture, const char* texture_filename, const char* texture_name, int texture_number)
{
	glActiveTexture(active_texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texture_filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	glUniform1i(glGetUniformLocation(shaderProgramID, texture_name), texture_number);

	stbi_image_free(data);
}

#pragma endregion TEXTURE_FUNCTIONS

void display() {
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);

	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(shaderProgramID, "model");
	int view_mat_location = glGetUniformLocation(shaderProgramID, "view");
	int proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");
	int texture_number = glGetUniformLocation(shaderProgramID, "texture_number");
	int light_x_loc = glGetUniformLocation(shaderProgramID, "light_x");
	int light_z_loc = glGetUniformLocation(shaderProgramID, "light_z");

	// Root of the Hierarchy
	mat4 view_mat = identity_mat4();
	mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);

	view_mat = rotate_y_deg(view_mat, 180.0f);
	view_mat = translate(view_mat, camera.pos);
	view_mat = rotate_y_deg(view_mat, camera.rot.v[Y]);

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);

	glUniform1f(light_x_loc, camera.pos.v[X]);
	glUniform1f(light_z_loc, camera.pos.v[Z]);

	// 
	// Road
	//
	mat4 road_model = identity_mat4();
	road_model = translate(road_model, road.pos);
	road_model = rotate_y_deg(road_model, 90.0f);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, road_model.m);
	glBindVertexArray(road.vao);
	glUniform1i(texture_number, ROAD_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[ROAD_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, road.model_data.mPointCount);


	// 
	// Ground
	//
	mat4 ground_model = identity_mat4();
	ground_model = translate(ground_model, ground.pos);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, ground_model.m);
	glBindVertexArray(ground.vao);
	glUniform1i(texture_number, GROUND_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[GROUND_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 2, ground.model_data.mPointCount);


	// 
	// Sky
	//
	mat4 sky_model = identity_mat4();
	sky_model = translate(sky_model, camera.pos);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, sky_model.m);
	glBindVertexArray(sky.vao);
	glUniform1i(texture_number, SKYBOX_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[SKYBOX_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, sky.model_data.mPointCount);

	// 
	// Car
	//
	mat4 car_model = identity_mat4();
	car_model = rotate_y_deg(car_model, car_body.rot.v[Y]);
	car_model = translate(car_model, car_body.pos);

	car_body.min_vec = car_model * car_body.min_vec;
	car_body.max_vec = car_model * car_body.max_vec;

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, car_model.m);
	glBindVertexArray(car_body.vao);
	glUniform1i(texture_number, RED_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[RED_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, car_body.model_data.mPointCount);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, car_model.m);
	glBindVertexArray(car_trim_and_windows.vao);
	glUniform1i(texture_number, BLACK_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[BLACK_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, car_trim_and_windows.model_data.mPointCount);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, car_model.m);
	glBindVertexArray(car_lights.vao);
	glUniform1i(texture_number, WHITE_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[WHITE_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, car_lights.model_data.mPointCount);


	//
	// Wheels
	//

	// Front Left
	mat4 wheel_model = identity_mat4();
	wheel_model = rotate_x_deg(wheel_model, fl_wheel_rim.rot.v[X]);
	wheel_model = rotate_y_deg(wheel_model, fl_wheel_rim.rot.v[Y]);
	wheel_model = translate(wheel_model, fl_wheel_rim.pos);

	wheel_model = car_model * wheel_model;

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(fl_wheel_rim.vao);
	glUniform1i(texture_number, GREY_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[GREY_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, fl_wheel_rim.model_data.mPointCount);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(fl_wheel_tyre.vao);
	glUniform1i(texture_number, BLACK_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[BLACK_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, fl_wheel_tyre.model_data.mPointCount);

	// Front Right
	wheel_model = identity_mat4();
	wheel_model = rotate_x_deg(wheel_model, fr_wheel_rim.rot.v[X]);
	wheel_model = rotate_y_deg(wheel_model, fr_wheel_rim.rot.v[Y]);
	wheel_model = translate(wheel_model, fr_wheel_rim.pos);

	wheel_model = car_model * wheel_model;

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(fr_wheel_rim.vao);
	glUniform1i(texture_number, GREY_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[GREY_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, fr_wheel_rim.model_data.mPointCount);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(fr_wheel_tyre.vao);
	glUniform1i(texture_number, BLACK_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[BLACK_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, fr_wheel_tyre.model_data.mPointCount);

	// Back Left
	wheel_model = identity_mat4();
	wheel_model = rotate_x_deg(wheel_model, bl_wheel_rim.rot.v[X]);
	wheel_model = translate(wheel_model, bl_wheel_rim.pos);

	wheel_model = car_model * wheel_model;

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(bl_wheel_rim.vao);
	glUniform1i(texture_number, GREY_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[GREY_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, bl_wheel_rim.model_data.mPointCount);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(bl_wheel_tyre.vao);
	glUniform1i(texture_number, BLACK_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[BLACK_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, bl_wheel_tyre.model_data.mPointCount);

	// Back Right
	wheel_model = identity_mat4();
	wheel_model = rotate_x_deg(wheel_model, br_wheel_rim.rot.v[X]);
	wheel_model = translate(wheel_model, br_wheel_rim.pos);

	wheel_model = car_model * wheel_model;

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(br_wheel_rim.vao);
	glUniform1i(texture_number, GREY_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[GREY_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, br_wheel_rim.model_data.mPointCount);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(br_wheel_tyre.vao);
	glUniform1i(texture_number, BLACK_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[BLACK_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, br_wheel_tyre.model_data.mPointCount);


	// 
	// AI Car
	//
	car_model = identity_mat4();
	car_model = rotate_y_deg(car_model, 180.0f);
	car_model = translate(car_model, ai_car_body.pos);

	ai_car_body.min_vec = car_model * ai_car_body.min_vec;
	ai_car_body.max_vec = car_model * ai_car_body.max_vec;

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, car_model.m);
	glBindVertexArray(ai_car_body.vao);
	glUniform1i(texture_number, GREEN_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[GREEN_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, ai_car_body.model_data.mPointCount);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, car_model.m);
	glBindVertexArray(ai_car_trim_and_windows.vao);
	glUniform1i(texture_number, BLACK_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[BLACK_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, ai_car_trim_and_windows.model_data.mPointCount);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, car_model.m);
	glBindVertexArray(ai_car_lights.vao);
	glUniform1i(texture_number, WHITE_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[WHITE_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, ai_car_lights.model_data.mPointCount);


	//
	// AI Wheels
	//

	// Front Left
	wheel_model = identity_mat4();
	wheel_model = rotate_x_deg(wheel_model, ai_fl_wheel_rim.rot.v[X]);
	wheel_model = rotate_y_deg(wheel_model, ai_fl_wheel_rim.rot.v[Y]);
	wheel_model = translate(wheel_model, ai_fl_wheel_rim.pos);

	wheel_model = car_model * wheel_model;

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(ai_fl_wheel_rim.vao);
	glUniform1i(texture_number, GREY_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[GREY_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, ai_fl_wheel_rim.model_data.mPointCount);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(ai_fl_wheel_tyre.vao);
	glUniform1i(texture_number, BLACK_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[BLACK_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, ai_fl_wheel_tyre.model_data.mPointCount);

	// Front Right
	wheel_model = identity_mat4();
	wheel_model = rotate_x_deg(wheel_model, ai_fr_wheel_rim.rot.v[X]);
	wheel_model = rotate_y_deg(wheel_model, ai_fr_wheel_rim.rot.v[Y]);
	wheel_model = translate(wheel_model, ai_fr_wheel_rim.pos);

	wheel_model = car_model * wheel_model;

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(ai_fr_wheel_rim.vao);
	glUniform1i(texture_number, GREY_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[GREY_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, ai_fr_wheel_rim.model_data.mPointCount);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(ai_fr_wheel_tyre.vao);
	glUniform1i(texture_number, BLACK_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[BLACK_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, ai_fr_wheel_tyre.model_data.mPointCount);

	// Back Left
	wheel_model = identity_mat4();
	wheel_model = rotate_x_deg(wheel_model, ai_bl_wheel_rim.rot.v[X]);
	wheel_model = translate(wheel_model, ai_bl_wheel_rim.pos);

	wheel_model = car_model * wheel_model;

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(ai_bl_wheel_rim.vao);
	glUniform1i(texture_number, GREY_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[GREY_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, ai_bl_wheel_rim.model_data.mPointCount);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(ai_bl_wheel_tyre.vao);
	glUniform1i(texture_number, BLACK_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[BLACK_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, ai_bl_wheel_tyre.model_data.mPointCount);

	// Back Right
	wheel_model = identity_mat4();
	wheel_model = rotate_x_deg(wheel_model, ai_br_wheel_rim.rot.v[X]);
	wheel_model = translate(wheel_model, ai_br_wheel_rim.pos);

	wheel_model = car_model * wheel_model;

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(ai_br_wheel_rim.vao);
	glUniform1i(texture_number, GREY_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[GREY_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, ai_br_wheel_rim.model_data.mPointCount);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wheel_model.m);
	glBindVertexArray(ai_br_wheel_tyre.vao);
	glUniform1i(texture_number, BLACK_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, textures[BLACK_TEXTURE]);
	glDrawArrays(GL_TRIANGLES, 0, ai_br_wheel_tyre.model_data.mPointCount);


	// 
	// Trees
	//
	for (size_t i = 0; i < NUMBER_OF_TREES; i++)
	{
		float new_x = 10.0f * sinf(tree_leaves.rot.v[Y] * M_PI / 180.0f) * i;
		float new_z = 10.0f * cosf(tree_leaves.rot.v[Y] * M_PI / 180.0f) * i;
		vec3 tree_pos = tree_leaves.pos + vec3(new_x, 0.0f, new_z);
		mat4 tree_model = identity_mat4();
		tree_model = scale(tree_model, tree_leaves.scale);
		tree_model = translate(tree_model, tree_pos);

		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, tree_model.m);
		glBindVertexArray(tree_leaves.vao);
		glUniform1i(texture_number, GREEN_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, textures[GREEN_TEXTURE]);
		glDrawArrays(GL_TRIANGLES, 0, tree_leaves.model_data.mPointCount);

		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, tree_model.m);
		glBindVertexArray(tree_trunk.vao);
		glUniform1i(texture_number, BROWN_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, textures[BROWN_TEXTURE]);
		glDrawArrays(GL_TRIANGLES, 0, tree_trunk.model_data.mPointCount);
	}

	glutSwapBuffers();
}


bool collision(Model* model1, Model* model2)
{
	bool result = (&model1->max_vec.v[0] >= &model2->min_vec.v[0] && &model2->max_vec.v[0] >= &model1->min_vec.v[0]) &&
		(&model1->max_vec.v[1] >= &model2->min_vec.v[1] && &model2->max_vec.v[1] >= &model1->min_vec.v[1]) &&
		(&model1->max_vec.v[2] >= &model2->min_vec.v[2] && &model2->max_vec.v[2] >= &model1->min_vec.v[2]);
	return result;
}


void updateScene() {
	if (!collision(&ai_car_body, &car_body))
	{
		ai_car_body.update();
	}
	car_body.update();
	fl_wheel_rim.update(&car_body, true);
	fr_wheel_rim.update(&car_body, false);
	bl_wheel_rim.update(&car_body, true);
	br_wheel_rim.update(&car_body, false);
	ai_fl_wheel_rim.update(&ai_car_body, true);
	ai_fr_wheel_rim.update(&ai_car_body, false);
	ai_bl_wheel_rim.update(&ai_car_body, true);
	ai_br_wheel_rim.update(&ai_car_body, false);
	camera.update();

	glutPostRedisplay();
}


void init()
{
	// Set up the shaders
	GLuint shaderProgramID = CompileShaders();

	glGenVertexArrays(1, &road.vao);
	generateObjectBufferMesh(road);

	glGenVertexArrays(1, &ground.vao);
	generateObjectBufferMesh(ground);

	glGenVertexArrays(1, &sky.vao);
	generateObjectBufferMesh(sky);

	// Car
	glGenVertexArrays(1, &car_body.vao);
	generateObjectBufferMesh(car_body);	
	glGenVertexArrays(1, &car_trim_and_windows.vao);
	generateObjectBufferMesh(car_trim_and_windows);
	glGenVertexArrays(1, &car_lights.vao);
	generateObjectBufferMesh(car_lights);

	// Wheels
	glGenVertexArrays(1, &fl_wheel_rim.vao);
	generateObjectBufferMesh(fl_wheel_rim);
	glGenVertexArrays(1, &fr_wheel_rim.vao);
	generateObjectBufferMesh(fr_wheel_rim);
	glGenVertexArrays(1, &bl_wheel_rim.vao);
	generateObjectBufferMesh(bl_wheel_rim);
	glGenVertexArrays(1, &br_wheel_rim.vao);
	generateObjectBufferMesh(br_wheel_rim);

	glGenVertexArrays(1, &fl_wheel_tyre.vao);
	generateObjectBufferMesh(fl_wheel_tyre);
	glGenVertexArrays(1, &fr_wheel_tyre.vao);
	generateObjectBufferMesh(fr_wheel_tyre);
	glGenVertexArrays(1, &bl_wheel_tyre.vao);
	generateObjectBufferMesh(bl_wheel_tyre);
	glGenVertexArrays(1, &br_wheel_tyre.vao);
	generateObjectBufferMesh(br_wheel_tyre);

	// AI Car
	glGenVertexArrays(1, &ai_car_body.vao);
	generateObjectBufferMesh(ai_car_body);
	glGenVertexArrays(1, &ai_car_trim_and_windows.vao);
	generateObjectBufferMesh(ai_car_trim_and_windows);
	glGenVertexArrays(1, &ai_car_lights.vao);
	generateObjectBufferMesh(ai_car_lights);

	// AI Wheels
	glGenVertexArrays(1, &ai_fl_wheel_rim.vao);
	generateObjectBufferMesh(ai_fl_wheel_rim);
	glGenVertexArrays(1, &ai_fr_wheel_rim.vao);
	generateObjectBufferMesh(ai_fr_wheel_rim);
	glGenVertexArrays(1, &ai_bl_wheel_rim.vao);
	generateObjectBufferMesh(ai_bl_wheel_rim);
	glGenVertexArrays(1, &ai_br_wheel_rim.vao);
	generateObjectBufferMesh(ai_br_wheel_rim);

	glGenVertexArrays(1, &ai_fl_wheel_tyre.vao);
	generateObjectBufferMesh(ai_fl_wheel_tyre);
	glGenVertexArrays(1, &ai_fr_wheel_tyre.vao);
	generateObjectBufferMesh(ai_fr_wheel_tyre);
	glGenVertexArrays(1, &ai_bl_wheel_tyre.vao);
	generateObjectBufferMesh(ai_bl_wheel_tyre);
	glGenVertexArrays(1, &ai_br_wheel_tyre.vao);
	generateObjectBufferMesh(ai_br_wheel_tyre);

	glGenVertexArrays(1, &tree_leaves.vao);
	generateObjectBufferMesh(tree_leaves);
	tree_leaves.scale = vec3(0.05f, 0.05f, 0.05f);

	glGenVertexArrays(1, &tree_trunk.vao);
	generateObjectBufferMesh(tree_trunk);
	tree_leaves.scale = vec3(0.05f, 0.05f, 0.05f);

	camera.lock_cam(&car_body);

	glGenTextures(9, textures);

	loadTexture(GL_TEXTURE0, textures[ROAD_TEXTURE], ROAD_TEXTURE_FILENAME, ROAD_TEXTURE_NAME, ROAD_TEXTURE);

	loadTexture(GL_TEXTURE1, textures[SKYBOX_TEXTURE], SKYBOX_TEXTURE_FILENAME, SKYBOX_TEXTURE_NAME, SKYBOX_TEXTURE);

	loadTexture(GL_TEXTURE2, textures[GROUND_TEXTURE], GROUND_TEXTURE_FILENAME, GROUND_TEXTURE_NAME, GROUND_TEXTURE);

	loadTexture(GL_TEXTURE3, textures[RED_TEXTURE], RED_TEXTURE_FILENAME, RED_TEXTURE_NAME, RED_TEXTURE);

	loadTexture(GL_TEXTURE4, textures[BLACK_TEXTURE], BLACK_TEXTURE_FILENAME, BLACK_TEXTURE_NAME, BLACK_TEXTURE);

	loadTexture(GL_TEXTURE5, textures[WHITE_TEXTURE], WHITE_TEXTURE_FILENAME, WHITE_TEXTURE_NAME, WHITE_TEXTURE);

	loadTexture(GL_TEXTURE6, textures[GREY_TEXTURE], GREY_TEXTURE_FILENAME, GREY_TEXTURE_NAME, GREY_TEXTURE);

	loadTexture(GL_TEXTURE7, textures[BROWN_TEXTURE], BROWN_TEXTURE_FILENAME, BROWN_TEXTURE_NAME, BROWN_TEXTURE);

	loadTexture(GL_TEXTURE8, textures[GREEN_TEXTURE], GREEN_TEXTURE_FILENAME, GREEN_TEXTURE_NAME, GREEN_TEXTURE);
}

#pragma region KEYBOARD FUNCTIONS
void keyDown(unsigned char key, int x, int y) {
	if (key == 27)
	{
		exit(0);
	}

	// Move car when cam_lock is on 
	if (camera.cam_lock) {
		if (key == 'w') {
			car_body.isMoving = true;
			car_body.vel.v[X] = 0.05f;
			car_body.vel.v[Z] = 0.05f;
		}
		if (key == 's') {
			car_body.isMoving = true;
			car_body.vel.v[X] = -0.05f;
			car_body.vel.v[Z] = -0.05f;
		}
		if (key == 'a') {
			car_body.isTurningLeft = true;
			car_body.rot_vel.v[Y] = 0.25f;
		}
		if (key == 'd') {
			car_body.isTurningRight = true;
			car_body.rot_vel.v[Y] = -0.25f;
		}
	}

	// Flip cam_lock
	if (key == 'c') {
		if (camera.cam_lock) {
			camera.unlock_cam();
		}
		else {
			camera.lock_cam(&car_body);
		}
	}

	// Move view when cam_lock is off
	if (!camera.cam_lock) {
		if (key == 'w') {
			camera.isMoving = true;
			camera.vel.v[X] = -0.05f;
			camera.vel.v[Z] = -0.05f;
		}
		if (key == 's') {
			camera.isMoving = true;
			camera.vel.v[X] = 0.05f;
			camera.vel.v[Z] = 0.05f;
		}
		if (key == 'a') {
			camera.rot_vel.v[Y] = -0.25f;
		}
		if (key == 'd') {
			camera.rot_vel.v[Y] = 0.25f;
		}
		if (key == 'q') {
			camera.vel.v[Y] = -0.05f;
		}
		if (key == 'e') {
			camera.vel.v[Y] = 0.05f;
		}
	}
}

void keyUp(unsigned char key, int x, int y) {
	if (key == 'w' || key == 's') {
		car_body.isMoving = false;
		car_body.vel.v[X] = 0.0f;
		car_body.vel.v[Z] = 0.0f;

		camera.isMoving = false;
		camera.vel.v[X] = 0.0f;
		camera.vel.v[Z] = 0.0f;
	}
	if (key == 'a' || key == 'd') {
		car_body.isTurningLeft = false;
		car_body.isTurningRight = false;

		car_body.rot_vel.v[Y] = 0.0f;
		camera.rot_vel.v[Y] = 0.0f;
	}
	if (key == 'q' || key == 'e') {
		camera.vel.v[Y] = 0.0f;
	}
}
#pragma endregion KEYBOARD FUNCTIONS

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Assignment");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	glewExperimental = GL_TRUE;
	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
