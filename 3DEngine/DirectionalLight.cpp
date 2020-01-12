#include "DirectionalLight.h"
#include "VecMath.h"
#include <math.h>

#include <iostream>

DirectionalLight::DirectionalLight()
{
}

Color DirectionalLight::CalculateDiffuse(const Color& diffuseColor, const Vec4& normal, const Vec4& toLight) const {

	double lightFactor = normal * toLight;
	if (lightFactor < 0) {
		lightFactor = 0;
	}

	return diffuseColor * color * lightFactor;

}

Color DirectionalLight::CalculateSpecular(const Color& specularColor, const Vec4& normal, const Vec4& toLight, 
										  const Vec4& toCamera, double specExp) const 
{

	double lightFactor = normal * toLight;
	if (lightFactor < 0) {
		lightFactor = 0;
	}

	int specularFactor = 1;
	if (lightFactor == 0) {
		specularFactor = 0;
	}

	Vec4 reflectionDirection = VecMath::Reflect(toLight, normal);
	reflectionDirection.Normalize();

	return specularColor * pow(fmax(reflectionDirection * toCamera, 0), specExp) * specularFactor;

}

DirectionalLight::~DirectionalLight()
{
}
