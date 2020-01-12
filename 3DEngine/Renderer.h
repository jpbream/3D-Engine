#pragma once
#include "Surface.h"
#include "Entity.h"
#include "Vec2.h"
#include "Color.h"
#include "Vec4.h"
#include "Camera.h"
#include <vector>
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

#define TOP_CLIP_PLANE 1
#define BOTTOM_CLIP_PLANE -1
#define NEAR_CLIP_PLANE -1
#define FAR_CLIP_PLANE 1
#define LEFT_CLIP_PLANE -1
#define RIGHT_CLIP_PLANE 1

class Renderer {

public:
	Renderer(Surface* s);
	~Renderer();
	void BeginDraw();
	void EndDraw();
	void DrawEntity(Entity* e, const Texture* const shadowMap, const Mat* const shadowCameraInverse);

	const Camera* camera;

	std::vector<const PointLight*> pointLights;
	std::vector<const DirectionalLight*> directionalLights;
	std::vector<const SpotLight*> spotLights;

	double ambientIntensity = 1;
	int renderMode = 0;

private:

	Surface* surface;
	double* zBuffer;

	const Texture* shadowMap;

	void TestAndSetPixel(int x, int y, double z, Color& c);

	void CullAndClip(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity);

	void ClipNear(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity, int planeID);
	void ClipFar(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity, int planeID);
	void ClipLeft(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity, int planeID);
	void ClipRight(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity, int planeID);
	void ClipBottom(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity, int planeID);
	void ClipTop(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity, int planeID);

	void Clip1(Vertex& outside, Vertex& inside1, Vertex& inside2, Face* face, Entity* entity, int planeID);
	void Clip2(Vertex& outside1, Vertex& outside2, Vertex& inside, Face* face, Entity* entity, int planeID);

	void FinishClipping(Vertex& v1, Vertex& v2, Vertex& v3, Face* face, Entity* entity, int planeID);

	void FillTriangle(Vertex vertex1, Vertex vertex2, Vertex vertex3, Face* face, Entity* entity);
	void FillFlatBottom(Vertex& topVertex, Vertex& leftVertex, Vertex& rightVertex, Face* face, Entity* entity);
	void FillFlatTop(Vertex& leftVertex, Vertex& rightVertex, Vertex& bottomVertex, Face* face, Entity* entity);

	Vec4 planeNormals[6];
	void(Renderer::*clipChain[7])(Vertex& v1, Vertex& v2, Vertex& v3, Face* f, Entity* e, int p) = 
			{&Renderer::ClipNear, &Renderer::ClipFar, &Renderer::ClipLeft, 
			&Renderer::ClipRight, &Renderer::ClipBottom, &Renderer::ClipTop, &Renderer::FinishClipping};

};
