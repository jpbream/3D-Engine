#pragma once
class Color
{

private:
	void Clamp();

public:

	double r;
	double g;
	double b;
	double a;

	Color();
	Color(double r, double g, double b);

	Color operator*(double scale) const;
	Color operator*(const Color& c) const;
	Color operator+(const Color& c) const;
	Color operator-(const Color& c) const;

	Color& operator+=(const Color& c);
	Color& operator-=(const Color& c);
	Color& operator*=(double s);
	Color& operator*=(const Color& c);

	int Compress() const;

	void Print();
};

