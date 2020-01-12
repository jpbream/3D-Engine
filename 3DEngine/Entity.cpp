#include "Mat.h"
#include "Entity.h"
#include "Quaternion.h"
#include "VecMath.h"
#include "CubeMap.h"
#include "Mat.h"

Entity::Entity(Model* model) : model(model) {
	
	std::cout << "Allocating " << sizeof(Vec4) * (model->numPositions + model->numNormals) / 1000000.0 << " megabytes for entity world space" << std::endl;

	worldSpace = new Vec4[model->numPositions];
	worldNormals = new Vec4[model->numNormals];
}

Entity::~Entity() {
	delete[] worldSpace;
	delete[] worldNormals;
}

Vec4& Entity::GetPosition(const Index& vertex) {

	if (vertex.positionIndex == NO_VALUE) {
		return noDataPosition;
	} 

	return worldSpace[vertex.positionIndex];
	
}

Vec2& Entity::GetTexel(const Index& vertex) {

	if (vertex.textureIndex == NO_VALUE) {
		return noDataTexel;
	} 

	return model->texels[vertex.textureIndex];
	
}

Vec4& Entity::GetNormal(const Index& vertex) {

	if (vertex.normalIndex == NO_VALUE) {
		return noDataNormal;
		//return (GetPosition(vertex) - position).Normalize();
	}

	return worldNormals[vertex.normalIndex];
}

Vertex Entity::GetVertex(const Index& vertex) {
	return Vertex(GetPosition(vertex), GetTexel(vertex), GetNormal(vertex), worldSpace[vertex.positionIndex], model->tangents[vertex.positionIndex]);
}

Material& Entity::GetMaterial(int ID) {

	if (ID == NO_VALUE	) {
		return noDataMaterial;
	}
	
	return model->materials[ID];
	
}

void Entity::VertexShader(Vertex& vertex, const Renderer* const renderer) {

}

void Entity::PixelShader(Color* out, const Vertex* const pixelVertex, const Renderer* const renderer, const Material* const material,
						const Texture* const shadowMap) {
	
	Vec4 toCamera = renderer->camera->position - pixelVertex->worldPosition;
	toCamera.Normalize();

	Vec4 normal;
	Mat objTotan(4, 4);
	Mat worldToobj(4, 4);
	if (!material->normalMap.IsNull()) {

		Color rawNormalSample;

		material->normalMap.Sample(pixelVertex->texel.x, pixelVertex->texel.y, &rawNormalSample);
		normal.x = rawNormalSample.r * 2 - 1;
		normal.y = rawNormalSample.g * 2 - 1;
		normal.z = rawNormalSample.b * 2 - 1;
		normal.w = 0;

		normal.Normalize();

		Vec4 tangent = pixelVertex->tangent;
		Vec4 bitangent = (normal % tangent) * tangent.w;

		//assemble matrix that is inverse (transpose) of the tangent space to object space matrix
		objTotan(0, 0) = tangent.x; objTotan(0, 1) = tangent.y; objTotan(0, 2) = tangent.z; objTotan(0, 3) = 0;
		objTotan(1, 0) = bitangent.x; objTotan(1, 1) = bitangent.y; objTotan(1, 2) = bitangent.z; objTotan(1, 3) = 0;
		objTotan(2, 0) = normal.x; objTotan(2, 1) = normal.y; objTotan(2, 2) = normal.z; objTotan(2, 3) = 0;
		objTotan(3, 0) = 0; objTotan(3, 1) = 0; objTotan(3, 2) = 0; objTotan(3, 3) = 0;

		//worldToobj = Mat::RotX(-rotation.x) * Mat::RotY(-rotation.y) * Mat::RotZ(-rotation.z);
		worldToobj = (Mat::RotX(rotation.x) * Mat::RotY(rotation.y) * Mat::RotZ(rotation.z)).Transpose();

		//need a world to obj matrix
		toCamera = objTotan * worldToobj * toCamera;

	}
	else {
		normal = pixelVertex->normal;
	}

	Color ambient;
	Color diffuse;
	Color specular;

	//ambient
	if (!material->ambientTexture.IsNull()) {
		material->ambientTexture.Sample(pixelVertex->texel.x, pixelVertex->texel.y, &ambient);
		ambient *= material->ambientColor;
	} else {
		ambient = material->ambientColor;
		//ambient = Color(0, 0, 0);
	}

	ambient *= renderer->ambientIntensity;

	//diffuse
	if (!material->diffuseTexture.IsNull()) {
		material->diffuseTexture.Sample(pixelVertex->texel.x, pixelVertex->texel.y, &diffuse);
	}
	else {
		diffuse = material->diffuseColor;
	}

	if (shiny == true) {

		Vec4 reflection = VecMath::Reflect(toCamera, normal);

		//Vec4 refraction = VecMath::Refract(toCamera, normal, 1, 1.52);

		cubeMap->Sample(reflection, &diffuse);
	}

	Color finalDiffuse = Color(0, 0, 0);
	Color finalSpecular = Color(0, 0, 0);

	//point lights
	for (int i = 0; i < renderer->pointLights.size(); i++) {

		const PointLight* p = renderer->pointLights[i];
		const Vec4& lightLocation = p->position;

		Vec4 toLight = lightLocation - pixelVertex->worldPosition;
		double distance = toLight.Magnitude();
		toLight.Normalize();

		if (!material->normalMap.IsNull()) {
			//transform toLight and toCamera into tangent space
			//toLight.print();
			toLight = objTotan * worldToobj * toLight;
			//std::cout << "\n";
			//toLight.print();
			//normal.print();
			//std::cout << "\n";
		}

		//diffuse
		finalDiffuse += p->CalculateDiffuse(diffuse, normal, toLight, distance);

		//specular
		finalSpecular += p->CalculateSpecular(material->specularColor, normal, toLight, toCamera, material->specularExponent, distance);

	}

	//directional lights
	for (int i = 0; i < renderer->directionalLights.size(); i++) {

		const DirectionalLight* p = renderer->directionalLights[i];

		Vec4 toLight = Mat::RotX(p->rotation.x) * Mat::RotY(p->rotation.y) * Mat::RotZ(p->rotation.z) * Vec4(0, 0, -1, 0) * -1;
	
		toLight.Normalize();

		if (!material->normalMap.IsNull()) {
			//transform toLight and toCamera into tangent space

			toLight = objTotan * worldToobj * toLight;
		}

		//diffuse
		finalDiffuse += p->CalculateDiffuse(diffuse, normal, toLight);

		//specular
		finalSpecular += p->CalculateSpecular(material->specularColor, normal, toLight, toCamera, material->specularExponent);

	}

	*out = ambient + finalDiffuse + finalSpecular + material->emissionColor;

	Color shadow;
	double depth = pixelVertex->shadow.z;
	if (shadowMap != nullptr && !shadowMap->IsNull() && renderer->renderMode == 0) {

		//pixelVertex->shadow.print();

		if (pixelVertex->shadow.x > 0 && pixelVertex->shadow.x < 1 && pixelVertex->shadow.y > 0 && pixelVertex->shadow.y < 1) {

			shadowMap->Sample(pixelVertex->shadow.x, pixelVertex->shadow.y, &shadow);
			double shadowDepth = shadow.r;

			if (depth > shadowDepth) {
				(*out).r *= 0.5;
				(*out).g *= 0.5;
				(*out).b *= 0.5;
			}
		}
	}
}

void Entity::Update(double dt) {

	//rotation.z -= 20 * dt;
	//rotation.y += 10 * dt;
	//rotation.z = 90;

}

void Entity::ApplyTransform() {

	//Mat rotX = Mat::RotX(rotation.x);
	//Mat rotY = Mat::RotY(rotation.y);
	//Mat rotZ = Mat::RotZ(rotation.z);
	//Mat rotProduct = rotX * rotY * rotZ;
	Quaternion rotX = Quaternion::RotX(rotation.x);
	Quaternion rotY = Quaternion::RotY(rotation.y);
	Quaternion rotZ = Quaternion::RotZ(rotation.z);
	Quaternion rotProduct = rotX * rotY * rotZ;
	Quaternion rotInverse = rotProduct.Inverse();

	Mat translation = Mat::Translation(position);

	for (int i = 0; i < model->numPositions; ++i) {

		//this is a shader, can't remember the type
		Vec4& vec = worldSpace[i];
		//vec = rotProduct * model->positions[i];

		//Quaternion multiplication
		vec = ((rotProduct * model->positions[i]) * rotInverse).ToVec4();

		/*vec.x += position.x;
		vec.y += position.y;
		vec.z += position.z;*/
		vec = translation * vec;
	}

	//will NOT work for scaling I believe
	for (int i = 0; i < model->numNormals; ++i) {

		Vec4& norm = worldNormals[i];

		norm = ((rotProduct * model->normals[i]) * rotInverse).ToVec4();

	}

}
