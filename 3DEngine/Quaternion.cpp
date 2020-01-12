#include "Quaternion.h"
#include "Math.h"

Quaternion::Quaternion(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

Quaternion Quaternion::RotX(double theta) {
	return Quaternion(Sin(theta * PI / 360), 0, 0, Cos(theta * PI / 360));
}

Quaternion Quaternion::RotY(double theta) {
	return Quaternion(0, Sin(theta * PI / 360), 0, Cos(theta * PI / 360));
}

Quaternion Quaternion::RotZ(double theta) {
	return Quaternion(0, 0, Sin(theta * PI / 360), Cos(theta * PI / 360));
}

//inputed values must be a UNIT VECTOR
Quaternion Quaternion::Rot(double x, double y, double z, double theta) {
	return Quaternion(x * Sin(theta * PI / 360), y * Sin(theta * PI / 360), z * Sin(theta * PI / 360), Cos(theta * PI / 360));
}

Quaternion Quaternion::operator*(const Vec4& v) {

	Quaternion q(v.x, v.y, v.z, 0);
	return Quaternion(w*q.x + x * q.w + y * q.z - z * q.y,
		w*q.y - x * q.z + y * q.w + z * q.x,
		w*q.z + x * q.y - y * q.x + z * q.w,
		w*q.w - x * q.x - y * q.y - z * q.z);

}

Quaternion Quaternion::operator*(const Quaternion& q) {

	return Quaternion(w*q.x + x * q.w + y * q.z - z * q.y,
		w*q.y - x * q.z + y * q.w + z * q.x,
		w*q.z + x * q.y - y * q.x + z * q.w,
		w*q.w - x * q.x - y * q.y - z * q.z);

}

Vec4 Quaternion::ToVec4() {
	return Vec4(x, y, z, 1);
}

Quaternion Quaternion::Inverse() {
	return Quaternion(-x, -y, -z, w);
}
