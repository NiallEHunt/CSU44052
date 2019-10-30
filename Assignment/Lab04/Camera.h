#pragma once

#define _USE_MATH_DEFINES

// Windows includes
#include <vector>

#include "maths_funcs.h"
#include "Model.h"

class Camera
{
public:
	Camera(vec3 starting_pos);
	vec3 pos;
	vec3 rot;
	vec3 vel;
	vec3 rot_vel;
	bool isMoving;
	bool cam_lock;
	void update();
	void lock_cam(Model* model_to_lock_onto);
	void unlock_cam();
private:
	Model* model;
};

