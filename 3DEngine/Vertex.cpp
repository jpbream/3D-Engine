#include "Vertex.h"

Vertex::Vertex() {}

Vertex::Vertex(const Vec4& position, const Vec2& texel, const Vec4& normal, const Vec4& worldPos, const Vec4& tangent)
	: renderPosition(position), texel(texel), normal(normal), worldPosition(worldPos), tangent(tangent)
{
}

Vertex::Vertex(const Vec4& position, const Vec2& texel, const Vec4& normal, const Vec4& worldPos, const Vec4& tangent, const Vec4& shadow)
	: renderPosition(position), texel(texel), normal(normal), worldPosition(worldPos), tangent(tangent), shadow(shadow)
{
}
