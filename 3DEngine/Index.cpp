#include "Index.h"

Index::Index() {}

Index::Index(Buffer index) : positionIndex(index), textureIndex(index), normalIndex(index) {}

Index::Index(int pos, int tex, int norm) : positionIndex(pos), textureIndex(tex), normalIndex(norm) {}

Index& Index::operator=(const Index& v) {
	positionIndex = v.positionIndex;
	textureIndex = v.textureIndex;
	normalIndex = v.normalIndex;

	return *this;
}