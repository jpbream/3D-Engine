#pragma once

class Vec4 {
public:
	Vec4();
	Vec4(double x, double y, double z, double w);

	double x;
	double y;
	double z;
	double w;

	void operator=(const Vec4&);
	void operator()(double, double, double, double);
	void operator()(Vec4&);

	Vec4 operator+(const Vec4&) const;
	Vec4& operator+=(const Vec4&);

	Vec4 operator-(const Vec4&) const;
	Vec4& operator-=(const Vec4&);

	Vec4 operator*(double) const;
	Vec4& operator*=(double);

	double Magnitude() const;
	Vec4& Normalize();

	double operator*(const Vec4&) const;
	Vec4 operator%(const Vec4&) const;

	void WDivide();

	bool operator==(const Vec4&) const;
	bool operator!=(const Vec4&) const;

	void print() const;
};
