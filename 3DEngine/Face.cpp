#include "Face.h"

Face::Face() {
	materialID = -1;
}
Face::Face(const Index& v1, const Index& v2, const Index& v3) : index1(v1), index2(v2), index3(v3) {
	materialID = -1;
}

Face& Face::operator=(const Face& f) {
	index1 = f.index1;
	index2 = f.index2;
	index3 = f.index3;

	materialID = f.materialID;

	return *this;
}