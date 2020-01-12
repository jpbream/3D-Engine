#pragma once
#include "Vec3.h"
#include "Vec4.h"
#include "Color.h"

class DirectionalLight
{
public:

	Vec3 rotation;
	Color color;

	Color CalculateDiffuse(const Color& diffuseColor, const Vec4& normal, const Vec4& toLight) const;
	Color CalculateSpecular(const Color& specularColor, const Vec4& normal, const Vec4& toLight, 
							const Vec4& toCamera, double specExp) const;

	DirectionalLight();
	~DirectionalLight();
};

