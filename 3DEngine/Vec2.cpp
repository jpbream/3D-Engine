#include <iostream>
#include "Math.h"
#include "Vec2.h"

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(double x, double y) : x(x), y(y) {}

void Vec2::operator()(double x, double y) {
	this->x = x;
	this->y = y;
}
void Vec2::operator()(Vec2& v) {
	x = v.x;
	y = v.y;
}

Vec2 Vec2::operator+(const Vec2& v) const {
	Vec2 r(this->x + v.x, this->y + v.y);
	return r;
}
Vec2& Vec2::operator+=(const Vec2& v) {
	this->x += v.x;
	this->y += v.y;

	return *this;
}

Vec2 Vec2::operator-(const Vec2& v) const {
	Vec2 r(this->x - v.x, this->y - v.y);
	return r;
}
Vec2& Vec2::operator-=(const Vec2& v) {
	this->x -= v.x;
	this->y -= v.y;

	return *this;
}

Vec2 Vec2::operator*(double s) const {
	Vec2 r(this->x * s, this->y * s);
	return r;
}
Vec2& Vec2::operator*=(double s) {
	this->x *= s;
	this->y *= s;

	return *this;
}

Vec2 Vec2::operator/(double s) const {
	return Vec2(x / s, y / s);
}

double Vec2::Magnitude() const {
	return Sqrt(x * x + y * y);
}
Vec2& Vec2::Normalize() {
	double mag = this->Magnitude();
	this->x /= mag;
	this->y /= mag;

	return *this;
}

double Vec2::operator*(const Vec2& v) const {
	return x * v.x + y * v.y;
}

bool Vec2::operator==(const Vec2& v) const {
	return x == v.x && y == v.y;
}
bool Vec2::operator!=(const Vec2& v) const {
	return !((*this) == v);
}

void Vec2::print() const {
	std::cout << "<" << x << ", " << y << ">" << std::endl;
}