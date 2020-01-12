#include <iostream>
#include "Math.h"
#include "Vec4.h"

Vec4::Vec4() : x(0), y(0), z(0), w(0) {}

Vec4::Vec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {};

void Vec4::operator=(const Vec4& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

void Vec4::operator()(double x, double y, double z, double w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
void Vec4::operator()(Vec4& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

Vec4 Vec4::operator+(const Vec4& v) const {
	return Vec4 (this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
}
Vec4& Vec4::operator+=(const Vec4& v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w += v.w;

	return *this;
}

Vec4 Vec4::operator-(const Vec4& v) const {
	return Vec4 (this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
}
Vec4& Vec4::operator-=(const Vec4& v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w -= v.w;

	return *this;
}

Vec4 Vec4::operator*(double s) const {
	return Vec4 (this->x * s, this->y * s, this->z * s, this->w * s);
}
Vec4& Vec4::operator*=(double s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;
	this->w *= s;

	return *this;
}

double Vec4::operator*(const Vec4& v) const {
	return x * v.x + y * v.y + z * v.z;
}

//cross product
Vec4 Vec4::operator%(const Vec4& v) const {
	return Vec4 (this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x, 0);
}

double Vec4::Magnitude() const {
	return Sqrt(x * x + y * y + z * z);
}
Vec4& Vec4::Normalize() {
	double mag = this->Magnitude();
	this->x /= mag;
	this->y /= mag;
	this->z /= mag;

	return *this;
}

void Vec4::WDivide() {
	x /= w;
	y /= w;
	z /= w;
}

bool Vec4::operator==(const Vec4& v) const {
	return (x == v.x && y == v.y && z == v.z && w == v.w);
}
bool Vec4::operator!=(const Vec4& v) const {
	return !((*this) == v);
}

void Vec4::print() const {
	std::cout << "<" << x << ", " << y << ", " << z << ", " << w << ">" << std::endl;
}