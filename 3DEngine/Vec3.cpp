#include <iostream>
#include "Math.h"
#include "Vec3.h"


Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

void Vec3::operator()(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
void Vec3::operator()(Vec3& v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

Vec3 Vec3::operator+(const Vec3& v) const {
	Vec3 r(this->x + v.x, this->y + v.y, this->z + v.z);
	return r;
}
Vec3& Vec3::operator+=(const Vec3& v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;

	return *this;
}

Vec3 Vec3::operator-(const Vec3& v) const {
	Vec3 r(this->x - v.x, this->y - v.y, this->z - v.z);
	return r;
}
Vec3& Vec3::operator-=(const Vec3& v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;

	return *this;
}

Vec3 Vec3::operator*(double s) const {
	Vec3 r(this->x * s, this->y * s, this->z * s);
	return r;
}
Vec3& Vec3::operator*=(double s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;

	return *this;
}

double Vec3::operator*(const Vec3& v) const {
	return x * v.x + y * v.y + z * v.z;
}

//cross product
Vec3 Vec3::operator%(const Vec3& v) const {
	Vec3 r(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x);
	return r;
}

double Vec3::Magnitude() const {
	return Sqrt(x * x + y * y + z * z);
}
Vec3& Vec3::Normalize() {
	double mag = this->Magnitude();
	this->x /= mag;
	this->y /= mag;
	this->z /= mag;

	return *this;
}

bool Vec3::operator==(const Vec3& v) const {
	return (x == v.x && y == v.y && z == v.z);
}
bool Vec3::operator!=(const Vec3& v) const {
	return !((*this) == v);
}

void Vec3::print() const {
	std::cout << "<" << x << ", " << y << ", " << z << ">" << std::endl;
}
