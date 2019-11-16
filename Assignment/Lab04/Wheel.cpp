#include "Wheel.h"

Wheel::Wheel(const char* mesh_name, vec3 starting_pos) : Model(mesh_name, starting_pos) { }

void Wheel::update(Model* parent)
{
	if (parent->isMoving)
	{
		float rot_x = rot.v[0] - 20.0f;

		rot.v[0] = fmodf(rot_x, 360.0f);
	}
}
