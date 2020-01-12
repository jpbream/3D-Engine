#include "PointLight.h"
#include "VecMath.h"
#include <math.h>



PointLight::PointLight()
{
}

double PointLight::GetIntensity(double distance) const {
	return 1.0 / (constant + linear * distance + quadratic * distance * distance);
}

Color PointLight::CalculateDiffuse(const Color& diffuseColor, const Vec4& normal, const Vec4& toLight, double distToLight) const {

	double lightFactor = normal * toLight;
	if (lightFactor < 0) {
		lightFactor = 0;
	}

	double intensity = GetIntensity(distToLight);
	Color lightAtFace = this->color * intensity;

	return diffuseColor * lightAtFace * lightFactor;

}

Color PointLight::CalculateSpecular(const Color& specularColor, const Vec4& normal, const Vec4& toLight,
	const Vec4& toCamera, double specExp, double distToLight) const 
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

	double intensity = GetIntensity(distToLight);

	return specularColor * intensity * pow(fmax(reflectionDirection * toCamera, 0), specExp) * specularFactor;

}


PointLight::~PointLight()
{
}
