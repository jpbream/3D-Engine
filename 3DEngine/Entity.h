#pragma once
#include "Vec3.h"
#include "Vec4.h"
#include "Model.h"
#include "Vertex.h"
#include "Index.h"
#include "Interpolator.h"
#include "Renderer.h"

class CubeMap;

class Entity {

private:
	Vec4& GetPosition(const Index& v);
	Vec2& GetTexel(const Index& v);
	Vec4& GetNormal(const Index& v);

public:

	CubeMap* cubeMap;
	bool shiny = false;

	Entity(Model* model);
	~Entity();

	Vec4 position;
	Vec3 rotation;
	Vec3 scale;

	Model* model;
	Vec4* worldSpace;
	Vec4* worldNormals;

	Vertex GetVertex(const Index& vertex);

	Material& GetMaterial(int ID);

	void VertexShader(Vertex& vertex, const Renderer* const renderer);
	void PixelShader(Color* out, const Vertex* const pixelData, const Renderer* const renderer, const Material* const material,
					const Texture* const shadowMap);

	void Update(double dt);
	void ApplyTransform();

};