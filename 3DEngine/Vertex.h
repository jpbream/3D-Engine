#pragma once
#include "Vec4.h"
#include "Vec2.h"

struct Vertex
{
public:

	Vertex();
	Vertex(const Vec4& renderPosition, const Vec2& texel, const Vec4& normal,
		const Vec4& worldPosition, const Vec4& tangent);
	Vertex(const Vec4& renderPosition, const Vec2& texel, const Vec4& normal, 
		   const Vec4& worldPosition, const Vec4& tangent, const Vec4& shadow);

	Vec4 renderPosition;
	Vec2 texel;
	Vec4 normal;
	Vec4 worldPosition;
	Vec4 tangent;
	Vec4 shadow;

	Vertex operator*(double s) {
		return Vertex(renderPosition * s, texel * s, normal * s, worldPosition * s, tangent * s, shadow * s);
	}

	Vertex operator+(const Vertex& vd) {
		return Vertex(renderPosition + vd.renderPosition, texel + vd.texel, normal + vd.normal, 
					  worldPosition + vd.worldPosition, tangent + vd.tangent, shadow + vd.shadow);
	}

	Vertex& operator+=(const Vertex& v) {
		renderPosition += v.renderPosition;
		texel += v.texel;
		normal += v.normal;
		worldPosition += v.worldPosition;
		tangent += v.tangent;
		shadow += v.shadow;

		return (*this);
	}

	void CorrectPerspective() {
		
		texel.x /= renderPosition.w;
		texel.y /= renderPosition.w;

		shadow.x /= renderPosition.w;
		shadow.y /= renderPosition.w;

		renderPosition.w = 1 / renderPosition.w;
	}

	void RecoverPerspective() {

		renderPosition.w = 1 / renderPosition.w;

		texel.x *= renderPosition.w;
		texel.y *= renderPosition.w;

		shadow.x *= renderPosition.w;
		shadow.y *= renderPosition.w;

	}
	
};

