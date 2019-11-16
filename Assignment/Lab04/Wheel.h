#pragma once
#include "Model.h"

class Wheel : public Model
{
public:
	Wheel(const char* mesh_name, vec3 starting_pos);
	void update(Model* parent);
};

