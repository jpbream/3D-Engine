#pragma once
#include "Vec4.h"
#include "Color.h"

class PointLight
{

public:

	double constant;
	double linear;
	double quadratic;

	Vec4 position;
	Color color;

	PointLight();
	~PointLight();

	double GetIntensity(double distance) const;

	Color CalculateDiffuse(const Color& diffuseColor, const Vec4& normal, const Vec4& toLight, double distToLight) const;
	Color CalculateSpecular(const Color& specularColor, const Vec4& normal, const Vec4& toLight, 
							const Vec4& toCamera, double specExp, double distToLight) const;
};

