#pragma once
#include "Vec4.h"

class VecMath
{
public:
	
	static Vec4 Reflect(const Vec4& v, const Vec4& normal);
	static Vec4 Refract(const Vec4& v, const Vec4& normal, double n1, double n2);

};

