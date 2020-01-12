#include "Color.h"
#include <math.h>
#include <iostream>

Color::Color() : r(0), g(0), b(0), a(1) {}

Color::Color(double r, double g, double b) : r(r), g(g), b(b) {
	a = 1;
}

Color Color::operator*(double scale) const {

	Color c(r * scale, g * scale, b * scale);
	c.Clamp();

	return c;
}

Color Color::operator*(const Color& c) const {

	Color co(r * c.r, g * c.g, b * c.b);
	co.Clamp();

	return co;

}

Color Color::operator+(const Color& c) const {

	Color k(r + c.r, g + c.g, b + c.b);
	k.Clamp();

	return k;

}
Color Color::operator-(const Color& c) const {

	Color k(r - c.r, g - c.g, b - c.b);
	k.Clamp();

	return k;

}

Color& Color::operator+=(const Color& c) {
	r += c.r;
	g += c.g;
	b += c.b;

	Clamp();
	return *this;
}
Color& Color::operator-=(const Color& c) {
	r -= c.r;
	g -= c.g;
	b -= c.b;

	Clamp();
	return *this;
}
Color& Color::operator*=(double s) {
	r *= s;
	g *= s;
	b *= s;

	Clamp();
	return *this;
}

Color& Color::operator*=(const Color& c) {
	r *= c.r;
	g *= c.g;
	b *= c.b;

	Clamp();
	return *this;
}

int Color::Compress() const {
	return ((int)(r * 255) << 24) | ((int)(g * 255) << 16) | ((int)(b * 255) << 8) | (int)(a * 255);

}

void Color::Clamp() {
	r = fmin(r, 1);
	r = fmax(r, 0);

	g = fmin(g, 1);
	g = fmax(g, 0);

	b = fmin(b, 1);
	b = fmax(b, 0);

	a = fmin(a, 1);
	a = fmax(a, 0);
}

void Color::Print() {
	std::cout << "(" << r << ", " << g << ", " << b << ", " << a << ")" << std::endl;
}
