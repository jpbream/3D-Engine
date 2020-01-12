#pragma once

class Vec3 {

public:
	Vec3();
	Vec3(double x, double y, double z);

	double x;
	double y;
	double z;

	void operator()(double, double, double);
	void operator()(Vec3&);

	Vec3 operator+(const Vec3&) const;
	Vec3& operator+=(const Vec3&);

	Vec3 operator-(const Vec3&) const;
	Vec3& operator-=(const Vec3&);

	Vec3 operator*(double) const;
	Vec3& operator*=(double);

	double Magnitude() const;
	Vec3& Normalize();

	double operator*(const Vec3&) const;
	Vec3 operator%(const Vec3& v) const;

	bool operator==(const Vec3&) const;
	bool operator!=(const Vec3&) const;

	void print() const;
};
