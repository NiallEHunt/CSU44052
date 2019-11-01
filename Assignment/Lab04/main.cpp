#define _USE_MATH_DEFINES

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
#include "Camera.h"

#define X 0
#define Y 1
#define Z 2

#define NUMBER_OF_TREES 7

// Model names to be loaded
#define CAR_MESH_NAME "models/Car.dae"
#define ROAD_MESH_NAME "models/road.obj"
#define TREE_MESH_NAME "models/tree.obj"

using namespace std;
GLuint shaderProgramID;

int width = 1600;
int height = 900;

GLuint loc1, loc2, loc3;
bool cam_lock = true;

Model road(ROAD_MESH_NAME, vec3(0.0f, -0.2f, 0.0f));
Model car(CAR_MESH_NAME, vec3(0.0f, 0.0f, 0.0f));
Model tree(TREE_MESH_NAME, vec3(3.0f, 0.0f, -15.0f));
Camera camera(vec3(0.0f, -2.5f, -10.0f));

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
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);

	unsigned int vn_vbo = 0;
	glGenBuffers(1, &vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);

	//	This is for texture coordinates which you don't currently need, so I have commented it out
	/*unsigned int vt_vbo = 0;
	glGenBuffers (1, &vt_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	glBufferData (GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof (vec2), &model.model_data.mTextureCoords[0], GL_STATIC_DRAW);*/

	glBindVertexArray(model.vao);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//	This is for texture coordinates which you don't currently need, so I have commented it out
	/*glEnableVertexAttribArray (loc3);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);*/
}
#pragma endregion VBO_FUNCTIONS


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

	// Root of the Hierarchy
	mat4 view_mat = identity_mat4();
	mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	
	view_mat = rotate_y_deg(view_mat, 180.0f);
	view_mat = translate(view_mat, camera.pos);
	view_mat = rotate_y_deg(view_mat, camera.rot.v[Y]);

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);

	// 
	// Road
	//
	mat4 road_model = identity_mat4();
	road_model = translate(road_model, road.pos);
	road_model = rotate_y_deg(road_model, 90.0f);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, road_model.m);
	glBindVertexArray(road.vao);
	glDrawArrays(GL_TRIANGLES, 0, road.model_data.mPointCount);
	
	// 
	// Car
	//
	mat4 car_model = identity_mat4();
	car_model = rotate_y_deg(car_model, car.rot.v[1]);
	car_model = translate(car_model, car.pos);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, car_model.m);
	glBindVertexArray(car.vao);
	glDrawArrays(GL_TRIANGLES, 0, car.model_data.mPointCount);

	// 
	// Trees
	//
	for (size_t i = 0; i < NUMBER_OF_TREES; i++)
	{
		float new_x = 5.0f * sinf(tree.rot.v[Y] * M_PI / 180.0f) * i;
		float new_z = 5.0f * cosf(tree.rot.v[Y] * M_PI / 180.0f) * i;
		vec3 tree_pos = tree.pos + vec3(new_x, 0.0f, new_z);
		mat4 tree_model = identity_mat4();
		tree_model = scale(tree_model, tree.scale);
		tree_model = translate(tree_model, tree_pos);

		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, tree_model.m);
		glBindVertexArray(tree.vao);
		glDrawArrays(GL_TRIANGLES, 0, tree.model_data.mPointCount);
	}

	glutSwapBuffers();
}


void updateScene() {
	car.update();
	camera.update();

	glutPostRedisplay();
}


void init()
{
	// Set up the shaders
	GLuint shaderProgramID = CompileShaders();

	glGenVertexArrays(1, &road.vao);
	generateObjectBufferMesh(road);

	glGenVertexArrays(1, &car.vao);
	generateObjectBufferMesh(car);
	
	glGenVertexArrays(1, &tree.vao);
	generateObjectBufferMesh(tree);
	tree.scale = vec3(0.05f, 0.05f, 0.05f);

	camera.lock_cam(&car);
}

void keyDown(unsigned char key, int x, int y) {
	// Move car when cam_lock is on 
	if (camera.cam_lock) {
		if (key == 'w') {
			car.isMoving = true;
			car.vel.v[X] = 0.01f;
			car.vel.v[Z] = 0.01f;
		}
		if (key == 's') {
			car.isMoving = true;
			car.vel.v[X] = -0.01f;
			car.vel.v[Z] = -0.01f;
		}
		if (key == 'a') {
			car.rot_vel.v[Y] = 0.1f;
		}
		if (key == 'd') {
			car.rot_vel.v[Y] = -0.1f;
		}
	}
	
	// Flip cam_lock
	if (key == 'c') {
		if (camera.cam_lock) {
			camera.unlock_cam();
		}
		else {
			camera.lock_cam(&car);
		}
	}

	// Move view when cam_lock is off
	if (!camera.cam_lock) {
		if (key == 'w') {
			camera.isMoving = true;
			camera.vel.v[X] = -0.01f;
			camera.vel.v[Z] = -0.01f;
		}
		if (key == 's') {
			camera.isMoving = true;
			camera.vel.v[X] = 0.01f;
			camera.vel.v[Z] = 0.01f;
		}
		if (key == 'a') {
			camera.rot_vel.v[Y] = -0.1f;
		}
		if (key == 'd') {
			camera.rot_vel.v[Y] = 0.1f;
		}
		if (key == 'q') {
			camera.vel.v[Y] = -0.01f;
		}
		if (key == 'e') {
			camera.vel.v[Y] = 0.01f;
		}
	}
}

void keyUp(unsigned char key, int x, int y) {
	if (key == 'w' || key == 's') {
		car.isMoving = false;
		car.vel.v[X] = 0.0f;
		car.vel.v[Z] = 0.0f;
		
		camera.isMoving = false;
		camera.vel.v[X] = 0.0f;
		camera.vel.v[Z] = 0.0f;
	}
	if (key == 'a' || key == 'd') {
		car.rot_vel.v[Y] = 0.0f;
		camera.rot_vel.v[Y] = 0.0f;
	}
	if (key == 'q' || key == 'e') {
		camera.vel.v[Y] = 0.0f;
	}
}

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
