#include <math.h>
#include "Vec3.h"
#include "Vec4.h"
#include "Mat.h"
#include "Renderer.h"
#include <memory>
#include "Interpolator.h"
#include "Face.h"

Renderer::Renderer(Surface* s) : surface(s)
{

	std::cout << "Allocating " << (sizeof(double) * surface->NumPixels()) / 1000000.0 << " megabytes for Z Buffer" << std::endl;
	zBuffer = new double[surface->NumPixels()];
	//std::fill_n(zBuffer, surface->NumPixels(), 100000);
	memset(zBuffer, 0, surface->NumPixels() * sizeof(double));

	//clipping chain for all 6 faces
	//Near -> Far -> Left -> Right -> Bottom -> Top
	planeNormals[0] = Vec4(0, 0, -NEAR_CLIP_PLANE, 1);
	planeNormals[1] = Vec4(0, 0, -FAR_CLIP_PLANE, 1);
	planeNormals[2] = Vec4(-LEFT_CLIP_PLANE, 0, 0, 1);
	planeNormals[3] = Vec4(-RIGHT_CLIP_PLANE, 0, 0, 1);
	planeNormals[4] = Vec4(0, -BOTTOM_CLIP_PLANE, 0, 1);
	planeNormals[5] = Vec4(0, -TOP_CLIP_PLANE, 0, 1);

}
Renderer::~Renderer() {
	delete[] zBuffer;
}

void Renderer::BeginDraw() {

}

void Renderer::EndDraw() {
	memset(zBuffer, 0, surface->NumPixels() * sizeof(double));
}

void Renderer::DrawEntity(Entity* entity, const Texture* const shadowMap, const Mat* const inverseShadowCamera) {

	if (camera == nullptr) {
		std::cout << "Renderer has not been assigned a camera reference. Entity will not be drawn." << std::endl;
		return;
	}

	this->shadowMap = shadowMap;

	/*Vec4 cameraView(0, 0, -1, 0);
	Vec4 cameraUp(0, 1, 0, 0);

	cameraView = Mat::RotX(camera->rotation.x) * Mat::RotY(camera->rotation.y) * cameraView;
	cameraUp = Mat::RotZ(camera->rotation.z) * cameraUp;
	Vec4 cameraRight = cameraView % cameraUp;
	cameraUp = cameraRight % cameraView;

	cameraView.Normalize();
	cameraUp.Normalize();
	cameraRight.Normalize();

	//transpose of the matrix [camRight camUp -camView]
	Mat inverseCameraRotation(4, 4);
	inverseCameraRotation(0, 0) = cameraRight.x; inverseCameraRotation(0, 1) = cameraRight.y; inverseCameraRotation(0, 2) = cameraRight.z; inverseCameraRotation(0, 3) = 0;
	inverseCameraRotation(1, 0) = cameraUp.x; inverseCameraRotation(1, 1) = cameraUp.y; inverseCameraRotation(1, 2) = cameraUp.z; inverseCameraRotation(0, 3) = 0;
	inverseCameraRotation(2, 0) = -cameraView.x; inverseCameraRotation(2, 1) = -cameraView.y; inverseCameraRotation(2, 2) = -cameraView.z; inverseCameraRotation(2, 3) = 0;
	inverseCameraRotation(3, 0) = 0; inverseCameraRotation(3, 1) = 0; inverseCameraRotation(3, 2) = 0; inverseCameraRotation(3, 3) = 1;*/

	Mat inverseCameraRotation = (Mat::RotX(camera->rotation.x) * Mat::RotY(camera->rotation.y) * Mat::RotZ(camera->rotation.z)).Transpose();
	Mat inverseCameraTranslation = Mat::Translation(camera->position * -1);

	double n = 0.1;
	double f = 100;
	double fov = camera->fov;
	double ar = (double)surface->height / surface->width;

	Mat frustum = Mat::Frustum(n, f, fov, ar);
	Mat orthographic = Mat::Orthographic(0, 100, 20, -20, -20, 20);
	Mat cameraInverse = inverseCameraRotation * inverseCameraTranslation;

	Mat viewport(4, 4);
	viewport(0, 0) = 0.5; viewport(0, 3) = 0.5;
	viewport(1, 1) = 0.5; viewport(1, 3) = 0.5;
	viewport(2, 2) = 0.5; viewport(2, 3) = 0.5;
	viewport(3, 3) = 1;

	Mat shadow = viewport * orthographic;

	for (int i = 0; i < entity->model->numFaces; ++i) {

		//this whole loop is in WORLD SPACE
		Face& face = entity->model->faces[i];

		Vertex vertex1 = entity->GetVertex(face.index1);
		Vertex vertex2 = entity->GetVertex(face.index2);
		Vertex vertex3 = entity->GetVertex(face.index3);

		Vec4 normal = (vertex2.renderPosition - vertex1.renderPosition) % (vertex3.renderPosition - vertex1.renderPosition);
		Vec4 toCamera = (camera->position - vertex1.renderPosition).Normalize();

		normal.Normalize();
		face.normal = normal;

		//backface culling
		double camFactor = normal * toCamera;
		if (camFactor < 0 && renderMode == 0) {
			continue;
		}
		
		//to CLIP SPACE
		if (renderMode != 0) {
			vertex1.renderPosition = orthographic * cameraInverse * vertex1.renderPosition;
			vertex2.renderPosition = orthographic * cameraInverse * vertex2.renderPosition;
			vertex3.renderPosition = orthographic * cameraInverse * vertex3.renderPosition;
		}
		else {

			if (shadowMap != nullptr && inverseShadowCamera != nullptr) {

				vertex1.shadow = shadow * (*inverseShadowCamera) * vertex1.renderPosition;
				vertex2.shadow = shadow * (*inverseShadowCamera) * vertex2.renderPosition;
				vertex3.shadow = shadow * (*inverseShadowCamera) * vertex3.renderPosition;

				if (vertex1.shadow.x < 0) {
					vertex1.shadow.print();
				}
				if (vertex2.shadow.x < 0) {
					vertex2.shadow.print();
				}
				if (vertex3.shadow.x < 0) {
					vertex3.shadow.print();
				}

			}

			vertex1.renderPosition = frustum * cameraInverse * vertex1.renderPosition;
			vertex2.renderPosition = frustum * cameraInverse * vertex2.renderPosition;
			vertex3.renderPosition = frustum * cameraInverse * vertex3.renderPosition;

		}

		//could do height maps here


		//perform clipping then draw triangles
		//defined in Clipping.cpp
	
		
		CullAndClip(vertex1, vertex2, vertex3, &face, entity);
		//FillTriangle(vertex1, vertex2, vertex3, &face, entity);

	}

}

//inneficient copying
void Renderer::FillTriangle(Vertex vertex1, Vertex vertex2, Vertex vertex3, Face* face, Entity* entity) {

	//to NDC space

	vertex1.renderPosition.WDivide();
	vertex2.renderPosition.WDivide();
	vertex3.renderPosition.WDivide();

	//move vertices to screen space
	vertex1.renderPosition.x = (int)((vertex1.renderPosition.x + 1) * surface->width / 2);
	vertex1.renderPosition.y = (int)((-vertex1.renderPosition.y + 1) * surface->height / 2);

	vertex2.renderPosition.x = (int)((vertex2.renderPosition.x + 1) * surface->width / 2);
	vertex2.renderPosition.y = (int)((-vertex2.renderPosition.y + 1) * surface->height / 2);

	vertex3.renderPosition.x = (int)((vertex3.renderPosition.x + 1) * surface->width / 2);
	vertex3.renderPosition.y = (int)((-vertex3.renderPosition.y + 1) * surface->height / 2);

	Vertex* topVertex = &vertex1;
	Vertex* middleVertex = &vertex2;
	Vertex* bottomVertex = &vertex3;

	Vec4* top = &vertex1.renderPosition;
	Vec4* middle = &vertex2.renderPosition;
	Vec4* bottom = &vertex3.renderPosition;

	if (top->y == middle->y && middle->y == bottom->y) {
		return;
	}

	if (middle->y < top->y) {
		std::swap(top, middle);
		std::swap(topVertex, middleVertex);
	}
	if (bottom->y < middle->y) {
		std::swap(bottom, middle);
		std::swap(bottomVertex, middleVertex);
	}
	if (middle->y < top->y) {
		std::swap(middle, top);
		std::swap(middleVertex, topVertex);
	}

	double alpha = (middle->y - top->y) / (bottom->y - top->y);

	//correct perspective of each vertex
	topVertex->CorrectPerspective();
	middleVertex->CorrectPerspective();
	bottomVertex->CorrectPerspective();

	//interpolate between top and bottom
	Vertex cutVertex = (*topVertex) * (1 - alpha) + (*bottomVertex) * alpha;

	if (cutVertex.renderPosition.x > middle->x) {
		if (abs(top->y - middle->y) >= 1) {
			FillFlatBottom(*topVertex, *middleVertex, cutVertex, face, entity);
		}
		if (abs(middle->y - bottom->y) >= 1) {
			FillFlatTop(*middleVertex, cutVertex, *bottomVertex, face, entity);
		}
	}
	else {
		if (abs(top->y - middle->y) >= 1) {
			FillFlatBottom(*topVertex, cutVertex, *middleVertex, face, entity);
		}
		if (abs(middle->y - bottom->y) >= 1) {
			FillFlatTop(cutVertex, *middleVertex, *bottomVertex, face, entity);
		}
	}

}

void Renderer::FillFlatBottom(Vertex& topVertex, Vertex& leftVertex, Vertex& rightVertex, Face* face, Entity* entity) {

	int pixelTop = (int)ceil(topVertex.renderPosition.y - 0.5);
	int pixelBottom = (int)ceil(leftVertex.renderPosition.y - 0.5);

	if (pixelTop < 0 || pixelBottom > surface->GetHeight()) {
		std::cout << pixelTop << " " << pixelBottom << std::endl;
		return;
	}

	double maxX = topVertex.renderPosition.x > rightVertex.renderPosition.x ? topVertex.renderPosition.x : rightVertex.renderPosition.x;

	double deltaY = (double)abs((pixelTop - pixelBottom));

	Vertex left = topVertex;
	Vertex right = topVertex;
	
	const Material* const material = &entity->GetMaterial(face->materialID);

	for (int y = pixelTop; y < pixelBottom; ++y) {

		double alpha = (y - pixelTop) / deltaY;

		left = topVertex * (1 - alpha) + leftVertex * alpha;
		right = topVertex * (1 - alpha) + rightVertex * alpha;

		int pixelLeft = (int)ceil(left.renderPosition.x  - 0.5);
		int pixelRight = (int)ceil(right.renderPosition.x - 0.5);

		double deltaX = (double)(pixelRight - pixelLeft);

		for (int x = pixelLeft; x < pixelRight && x <= maxX; ++x) {
			
			double beta = (x - pixelLeft) / deltaX;

			Vertex across = left * (1 - beta) + right * beta;

			across.RecoverPerspective();

			//renormalize
			across.normal.Normalize();

			Color c;

			if (renderMode == 0) {
				entity->PixelShader(&c, &across, this, material, shadowMap);
			}
			else {
				double depth = across.renderPosition.z;

				c.r = (depth + 1) / 2;
				c.g = (depth + 1) / 2;
				c.b = (depth + 1) / 2;
				c.a = 1;
			}

			TestAndSetPixel(x, y, across.renderPosition.z, c);

			across.CorrectPerspective();
		}
	}

}

void Renderer::FillFlatTop(Vertex& leftVertex, Vertex& rightVertex, Vertex& bottomVertex, Face* face, Entity* entity) {


	if (renderMode == 0) {
		//leftVertex.shadow.print();
		//rightVertex.shadow.print();
		//bottomVertex.shadow.print();
	}

	int pixelTop = (int)ceil(leftVertex.renderPosition.y - 0.5);
	int pixelBottom = (int)ceil(bottomVertex.renderPosition.y - 0.5);

	if (pixelTop < 0 || pixelBottom > surface->GetHeight()) {
		std::cout << pixelTop << " " << pixelBottom << std::endl;
		return;
	}

	double maxX = bottomVertex.renderPosition.x > rightVertex.renderPosition.x ? bottomVertex.renderPosition.x : rightVertex.renderPosition.x;

	double deltaY = (double)abs((pixelTop - pixelBottom));

	Vertex left = leftVertex;
	Vertex right = rightVertex;

	const Material* const material = &entity->GetMaterial(face->materialID);

	for (int y = pixelTop; y < pixelBottom; ++y) {

		double alpha = (y - pixelTop) / deltaY;

		left = leftVertex * (1 - alpha) + bottomVertex * alpha;
		right = rightVertex * (1 - alpha) + bottomVertex * alpha;

		int pixelLeft = (int)ceil(left.renderPosition.x - 0.5);
		int pixelRight = (int)ceil(right.renderPosition.x - 0.5);

		double deltaX = (double)(pixelRight - pixelLeft);

		for (int x = pixelLeft; x < pixelRight && x <= maxX; ++x) {

			double beta = (x - pixelLeft) / deltaX;

			Vertex across = left * (1 - beta) + right * beta;

			across.RecoverPerspective();

			//renormalize
			across.normal.Normalize();
			
			Color c;

			if (renderMode == 0) {
				entity->PixelShader(&c, &across, this, material, shadowMap);
			}
			else {
				double depth = across.renderPosition.z;

				c.r = (depth + 1) / 2;
				c.g = (depth + 1) / 2;
				c.b = (depth + 1) / 2;
				c.a = 1;
			}
	
			TestAndSetPixel(x, y, across.renderPosition.z, c);

			across.CorrectPerspective();
		}
	}

}

void Renderer::TestAndSetPixel(int x, int y, double z, Color& c) {

	//using 1/(z+1) > is just a trick so memset can be used. In reality, a lower z value is closer to the camera
	if (x > 0 && x < surface->width && y > 0 && y < surface->height && 1.0 / (z + 1) > zBuffer[y * surface->width + x]) {

		zBuffer[y * surface->width + x] = 1.0 / (z + 1);
		surface->PutPixel(x, y, c);

	}

}
