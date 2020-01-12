#include "VecMath.h"
#include "Math.h"

Vec4 VecMath::Reflect(const Vec4& v, const Vec4& normal) {

	return normal * (normal * v * 2) - v;

}

Vec4 VecMath::Refract(const Vec4& v, const Vec4& normal, double n1, double n2) {

	double nRatio = n1 / n2;

	double coefficient = ((normal * nRatio) * v) - (Sqrt(1 - (nRatio * nRatio) * (1 - Pow(normal * v, 2))));

	return normal * coefficient - v * nRatio;

}
