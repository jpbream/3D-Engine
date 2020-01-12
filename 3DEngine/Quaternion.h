#pragma once
#include "Vec4.h"

class Quaternion
{
public:

	double w;
	double x;
	double y;
	double z;

	Quaternion(double x, double y, double z, double w);

	static Quaternion RotX(double theta);
	static Quaternion RotY(double theta);
	static Quaternion RotZ(double theta);

	static Quaternion Rot(double x, double y, double z, double theta);

	Quaternion operator*(const Vec4&);
	Quaternion operator*(const Quaternion&);

	Vec4 ToVec4();

	Quaternion Inverse();
};

