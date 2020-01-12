#include "Material.h"

Material::Material() {}

Material::Material(const Material& m) {

	//this is called somewhere in vector push back from iomodel
	TransferCopyables(*this, m);
	CopyFrom(m);
}

void Material::TransferCopyables(Material& dest, const Material& src) {

	dest.ambientColor = src.ambientColor;

	dest.diffuseColor = src.diffuseColor;

	dest.specularColor = src.specularColor;
	dest.specularExponent = src.specularExponent;

	dest.emissionColor = src.emissionColor;

}

void Material::CopyFrom(const Material& m) {
	
	TransferCopyables(*this, m);

	ambientTexture.CopyFrom(m.ambientTexture);
	diffuseTexture.CopyFrom(m.diffuseTexture);
	normalMap.CopyFrom(m.normalMap);

}

void Material::MoveFrom(Material& m) {

	TransferCopyables(*this, m);

	ambientTexture.MoveFrom(m.ambientTexture);
	diffuseTexture.MoveFrom(m.diffuseTexture);
	normalMap.MoveFrom(m.normalMap);

}

void Material::CopyTo(Material& m) {

	TransferCopyables(m, *this);

	ambientTexture.CopyTo(m.ambientTexture);
	diffuseTexture.CopyTo(m.diffuseTexture);
	normalMap.CopyTo(m.normalMap);

}

void Material::MoveTo(Material& m) {

	TransferCopyables(m, *this);

	ambientTexture.MoveTo(m.ambientTexture);
	diffuseTexture.MoveTo(m.diffuseTexture);
	normalMap.MoveTo(m.normalMap);

}

/*Material::~Material() {
	diffuseTexture.Delete();
	normalMap.Delete();
}*/

/*Material::Material(const Material& m) {

	ambientColor = m.ambientColor;

	diffuseColor = m.diffuseColor;
	diffuseTexture = m.diffuseTexture;

	normalMap = m.normalMap;

	specularColor = m.specularColor;
	specularExponent = m.specularExponent;

	emissionColor = m.emissionColor;

}*/


