#include "Camera.h"

Camera::Camera(vec3 starting_pos)
{
	pos = starting_pos;
	isMoving = false;
	cam_lock = false;
}

void Camera::update()
{
	if (cam_lock){
		pos.v[2] = model->pos.v[2] - (10.0f * cosf(model->rot.v[1] * M_PI / 180.0f));
		pos.v[0] = model->pos.v[0] - (10.0f * sinf(model->rot.v[1] * M_PI / 180.0f));

		rot.v[1] = -model->rot.v[1];
	}
	else {
		vec3 new_vel = vel;

		if (isMoving) {
			new_vel.v[0] = vel.v[0] * -sinf(-rot.v[1] * M_PI / 180.0f);
			new_vel.v[2] = vel.v[2] * -cosf(-rot.v[1] * M_PI / 180.0f);
		}

		pos += new_vel;
		rot += rot_vel;
	}
}

void Camera::lock_cam(Model* model_to_lock_onto)
{
	model = model_to_lock_onto;
	cam_lock = true;
}

void Camera::unlock_cam()
{
	cam_lock = false;
}
