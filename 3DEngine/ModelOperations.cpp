#include "ModelOperations.h"
#include "Mat.h"
#include "Face.h"
#include "Index.h"

void ModelOperations::CalculateTangents(const Model& m) {

	Vec4* tan = new Vec4[m.numPositions];
	Vec4* bitan = new Vec4[m.numPositions];

	for (int i = 0; i < m.numFaces; i++) {

		int i1 = m.faces[i].index1.positionIndex;
		int i2 = m.faces[i].index2.positionIndex;
		int i3 = m.faces[i].index3.positionIndex;

		int t1 = m.faces[i].index1.textureIndex;
		int t2 = m.faces[i].index2.textureIndex;
		int t3 = m.faces[i].index3.textureIndex;

		const Vec4& v1 = m.positions[i1];
		const Vec4& v2 = m.positions[i2];
		const Vec4& v3 = m.positions[i3];

		const Vec2& w1 = m.texels[t1];
		const Vec2& w2 = m.texels[t2];
		const Vec2& w3 = m.texels[t3];

		Vec4 q1 = v2 - v1;
		Vec4 q2 = v3 - v1;

		Vec2 u = w2 - w1;
		Vec2 v = w3 - w1;

		Mat uv(2, 2);
		uv(0, 0) = v.y;
		uv(0, 1) = -v.x;
		uv(1, 0) = -u.y;
		uv(1, 1) = u.x;

		double detUV = (u.x * v.y - u.y * v.x);
		uv *= (1 / detUV);

		Mat q(2, 3);
		q(0, 0) = q1.x; q(0, 1) = q1.y; q(0, 2) = q1.z;
		q(1, 0) = q2.x; q(1, 1) = q2.y; q(1, 2) = q2.z;

		Mat tb = uv * q;

		Vec4 faceTan(tb(0, 0), tb(0, 1), tb(0, 2), 0);
		Vec4 faceBiTan(tb(1, 0), tb(1, 1), tb(1, 2), 0);

		tan[i1] += faceTan;
		bitan[i1] += faceBiTan;

		tan[i2] += faceTan;
		bitan[i2] += faceBiTan;

		tan[i3] += faceTan;
		bitan[i3] += faceBiTan;

	}

	for (int i = 0; i < m.numFaces; i++) {

		int i1 = m.faces[i].index1.positionIndex;
		int i2 = m.faces[i].index2.positionIndex;
		int i3 = m.faces[i].index3.positionIndex;

		if (tan[i1].x != -123456) {

			const Vec4& normal = m.normals[m.faces[i].index1.normalIndex];
			const Vec4& tangent = tan[i1];
			Vec4& bitangent = bitan[i1];

			//Gram-Schmidt orthogonalize
			m.tangents[i1] = (tangent - normal * (normal * tangent)).Normalize();
			bitan[i1] = (bitan[i1] - normal * (normal * bitan[i1]) - m.tangents[i1] * (m.tangents[i1] * bitangent)).Normalize();

			//Calculate handedness (determinant of object space to tangent space matrix)
			m.tangents[i1].w = (normal % m.tangents[i1]) * bitan[i1];

			tan[i1].x = -123456;

		}

		if (tan[i2].x != -123456) {

			const Vec4& normal = m.normals[m.faces[i].index2.normalIndex];
			const Vec4& tangent = tan[i2];
			Vec4& bitangent = bitan[i2];

			//Gram-Schmidt orthogonalize
			m.tangents[i2] = (tangent - normal * (normal * tangent)).Normalize();
			bitan[i2] = (bitan[i2] - normal * (normal * bitan[i2]) - m.tangents[i2] * (m.tangents[i2] * bitangent)).Normalize();

			//Calculate handedness (determinant of object space to tangent space matrix)
			m.tangents[i2].w = (normal % m.tangents[i2]) * bitan[i2];

			tan[i2].x = -123456;

		}

		if (tan[i3].x != -123456) {

			const Vec4& normal = m.normals[m.faces[i].index3.normalIndex];
			const Vec4& tangent = tan[i3];
			Vec4& bitangent = bitan[i3];

			//Gram-Schmidt orthogonalize
			m.tangents[i3] = (tangent - normal * (normal * tangent)).Normalize();
			bitan[i3] = (bitan[i3] - normal * (normal * bitan[i3]) - m.tangents[i3] * (m.tangents[i3] * bitangent)).Normalize();

			//Calculate handedness (determinant of object space to tangent space matrix)
			m.tangents[i3].w = (normal % m.tangents[i3]) * bitan[i3];

			tan[i3].x = -123456;

		}

	}

	delete[] tan;
	delete[] bitan;
}

