#pragma once
#include <iostream>
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

class Mat {

private:
	int rows;
	int cols;
	double* data;

public:

	Mat(int r, int c);
	Mat(Mat&&);
	Mat& operator=(Mat&&);

	~Mat();

	int NumRows() const {
		return rows;
	}
	int NumCols() const {
		return cols;
	}

	double operator()(int r, int c) const;
	double& operator()(int r, int c);

	Mat operator*(double s);
	Mat& operator*=(double s);

	Mat operator*(const Mat& m) const;
	Vec2 operator*(const Vec2&) const;
	Vec3 operator*(const Vec3&) const;
	Vec4 operator*(const Vec4&) const;

	Mat Transpose();

	static Mat RotX(double theta);
	static Mat RotY(double theta);
	static Mat RotZ(double theta);

	static Mat Translation(const Vec4& v);

	//static Mat Frustum(double n, double f, double l, double r, double t, double b);
	static Mat Frustum(double n, double f, double fov, double ar);
	static Mat Orthographic(double n, double f, double t, double b, double l, double r);

	friend std::ostream& operator<<(std::ostream& os, const Mat& m);
};





