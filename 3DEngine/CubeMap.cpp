#include "CubeMap.h"
#include "Mat.h"



CubeMap::CubeMap(Model* model) : Entity::Entity(model)
{

	NegX = &model->materials[0].ambientTexture;
	PosX = &model->materials[1].ambientTexture;
	PosZ = &model->materials[3].ambientTexture;
	NegZ = &model->materials[2].ambientTexture;
	PosY = &model->materials[4].ambientTexture;
	NegY = &model->materials[5].ambientTexture;

}

void CubeMap::Sample(const Vec4& dir, Color* color) {

	Vec4 direction = Mat::RotY(this->rotation.y) * dir;

	double x = abs(direction.x);
	double y = abs(direction.y);
	double z = abs(direction.z);

	double s = 0;
	double t = 0;

	if (x >= y && x >= z) {

		x = -direction.x;
		y = direction.y;
		z = direction.z;

		if (x > 0) {
			s = 0.5 - z / (2 * x);
			t = 0.5 - y / (2 * x);
			PosX->Sample(s, t, color);
		}
		else {
			s = 0.5 - z / (2 * x);
			t = 0.5 + y / (2 * x);
			NegX->Sample(s, t, color);
		}
	}
	if (y > x && y >= z) {

		x = -direction.x;
		y = direction.y;
		z = direction.z;

		if (y > 0) {
			s = 0.5 + x / (2 * y);
			t = 0.5 + z / (2 * y);
			PosY->Sample(s, t, color);
		}
		else {
			s = 0.5 - x / (2 * y);
			t = 0.5 + z / (2 * y);
			NegY->Sample(s, t, color);
		}
	}
	if (z > x && z > y) {

		x = -direction.x;
		y = direction.y;
		z = direction.z;

		if (z > 0) {
			s = 0.5 + x / (2 * z);
			t = 0.5 - y / (2 * z);
			PosZ->Sample(s, t, color);
		}
		else {
			s = 0.5 + x / (2 * z);
			t = 0.5 + y / (2 * z);
			NegZ->Sample(s, t, color);
		}
	}

}


CubeMap::~CubeMap()
{
}
