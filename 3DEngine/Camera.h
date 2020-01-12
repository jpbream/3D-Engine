#pragma once
#include "Vec4.h"
#include "Vec3.h"
#include "Mat.h"

class Camera
{

public:

	Vec4 position;
	Vec3 rotation;

	double fov = 150;

	Camera();
};

