#pragma once

class Vec2 {

public:
	Vec2();
	Vec2(double x, double y);

	double x;
	double y;

	void operator()(double, double);
	void operator()(Vec2&);

	Vec2 operator+(const Vec2&) const;
	Vec2& operator+=(const Vec2&);

	Vec2 operator-(const Vec2&) const;
	Vec2& operator-=(const Vec2&);

	Vec2 operator*(double) const;
	Vec2& operator*=(double);

	Vec2 operator/(double) const;

	double Magnitude() const;
	Vec2& Normalize();

	double operator*(const Vec2&) const;

	bool operator==(const Vec2&) const;
	bool operator!=(const Vec2&) const;

	void print() const;
};