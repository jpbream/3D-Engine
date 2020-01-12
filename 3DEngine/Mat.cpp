#include "Log.h"
#include <memory>
#include "Math.h"
#include "Mat.h"

Mat::Mat(int r, int c) : rows(r), cols(c) {
	data = new double[r * c];
	memset(data, 0, sizeof(double) * r * c);
}

// Move constructor
Mat::Mat(Mat&& a) : data(a.data)
{
	rows = a.rows;
	cols = a.cols;
	a.data = nullptr;
}

Mat& Mat::operator=(Mat&& a) {

	data = a.data;
	rows = a.rows;
	cols = a.cols;
	a.data = nullptr;

	return *this;

}

Mat::~Mat() {
	delete[] data;
}

double Mat::operator()(int r, int c) const {
	Log::Assert(r >= 0 && r < rows && c >= 0 && c < cols, "index position outside matrix");
	return data[r * cols + c];
}
double& Mat::operator()(int r, int c) {
	Log::Assert(r >= 0 && r < rows && c >= 0 && c < cols, "index position outside matrix");
	return data[r * cols + c];
}

Mat Mat::operator*(double s) {
	Mat m(rows, cols);

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			m(r, c) = (*this)(r, c) * s;
		}
	}

	return m;
}

Mat& Mat::operator*=(double s) {
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			(*this)(r, c) *= s;
		}
	}

	return *this;
}

Mat Mat::operator*(const Mat& m) const {

	Log::Assert(cols == m.rows, "matrices must be correct sizes to multiply");

	Mat product(rows, m.cols);

	int numParts = cols;

	//loop through row and cols in this
	for (int row = 0; row < rows; ++row) {
		
		//loop through cols in m
		for (int col = 0; col < m.cols; ++col) {

			double rowProduct = 0;
			for (int i = 0; i < numParts; ++i) {
				rowProduct += (*this)(row, i) * m(i, col);
			}

			product(row, col) = rowProduct;

		}
	}

	return product;
}

Vec2 Mat::operator*(const Vec2& v) const {

	Log::Assert(rows == 2 && cols == 2, "matrix must be a 2x2");

	Vec2 product;
	product.x = (*this)(0, 0) * v.x + (*this)(0, 1) * v.y;
	product.y = (*this)(1, 0) * v.x + (*this)(1, 1) * v.y;

	return product;

}

Vec3 Mat::operator*(const Vec3& v) const {

	Log::Assert(rows == 3 && cols == 3, "matrix must be a 3x3");

	Vec3 product;
	product.x = (*this)(0, 0) * v.x + (*this)(0, 1) * v.y + (*this)(0, 2) * v.z;
	product.y = (*this)(1, 0) * v.x + (*this)(1, 1) * v.y + (*this)(1, 2) * v.z;
	product.z = (*this)(2, 0) * v.x + (*this)(2, 1) * v.y + (*this)(2, 2) * v.z;

	return product;

}

Vec4 Mat::operator*(const Vec4& v) const {

	Log::Assert(rows == 4 && cols == 4, "matrix must be a 4x4");

	Vec4 product;
	product.x = (*this)(0, 0) * v.x + (*this)(0, 1) * v.y + (*this)(0, 2) * v.z + (*this)(0, 3) * v.w;
	product.y = (*this)(1, 0) * v.x + (*this)(1, 1) * v.y + (*this)(1, 2) * v.z + (*this)(1, 3) * v.w;
	product.z = (*this)(2, 0) * v.x + (*this)(2, 1) * v.y + (*this)(2, 2) * v.z + (*this)(2, 3) * v.w;
	product.w = (*this)(3, 0) * v.x + (*this)(3, 1) * v.y + (*this)(3, 2) * v.z + (*this)(3, 3) * v.w;

	return product;

}

Mat Mat::Transpose() {

	Mat transpose(rows, cols);
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			transpose(r, c) = (*this)(c, r);
		}
	}

	return transpose;

}

Mat Mat::RotX(double theta) {

	theta *= PI / 180;

	Mat m(4, 4);
	m(1, 1) = cos(theta);
	m(2, 1) = sin(theta);
	m(1, 2) = -sin(theta);
	m(2, 2) = cos(theta);
	m(0, 0) = 1;
	m(3, 3) = 1;

	return m;
}

Mat Mat::RotY(double theta) {

	theta *= PI / 180;

	Mat m(4, 4);
	m(0, 0) = cos(theta);
	m(2, 0) = sin(theta);
	m(0, 2) = -sin(theta);
	m(2, 2) = cos(theta);
	m(1, 1) = 1;
	m(3, 3) = 1;

	return m;
}

Mat Mat::RotZ(double theta) {

	theta *= PI / 180;

	Mat m(4, 4);
	m(0, 0) = cos(theta);
	m(1, 0) = sin(theta);
	m(0, 1) = -sin(theta);
	m(1, 1) = cos(theta);
	m(2, 2) = 1;
	m(3, 3) = 1;

	return m;
}

Mat Mat::Translation(const Vec4& v) {

	Mat m(4, 4);
	m(0, 0) = 1;
	m(1, 1) = 1;
	m(2, 2) = 1;
	m(3, 3) = 1;
	m(0, 3) = v.x;
	m(1, 3) = v.y;
	m(2, 3) = v.z;

	return m;

}

//Mat Mat::Frustum(double n, double f, double l, double r, double t, double b) {
Mat Mat::Frustum(double n, double f, double fov, double ar) {

	fov *= PI / 180;
	double e = 1.0 / Tan(fov / 2);
	double l = -n / e;
	double r = n / e;
	double t = ar * n / e;
	double b = -ar * n / e;


	Mat m(4, 4);

	m(0, 0) = 2 * n / (r - l);
	m(0, 2) = (r + l) / (r - l);
	m(1, 1) = 2 * n / (t - b);
	m(1, 2) = (t + b) / (t - b);
	m(2, 2) = -(f + n) / (f - n);
	m(2, 3) = -(2 * f * n) / (f - n);
	m(3, 2) = -1;

	return m;
}

Mat Mat::Orthographic(double n, double f, double t, double b, double l, double r) {

	Mat m(4, 4);

	m(0, 0) = 2 / (r - l);	m(0, 3) = -(r + l) / (r - l);
	m(1, 1) = 2 / (t - b);	m(1, 3) = -(t + b) / (t - b);
	m(2, 2) = -2 / (f - n);	m(2, 3) = -(f + n) / (f - n);
	m(3, 3) = 1;

	return m;

}

std::ostream& operator<<(std::ostream& os, const Mat& m) {
	
	for (int r = 0; r < m.rows; ++r) {
		os << "[";
		for (int c = 0; c < m.cols; ++c) {
			os << m(r, c);

			if (c != m.cols - 1) {
				os << ", ";
			}
		}
		os << "]\n";
	}

	return os;

}