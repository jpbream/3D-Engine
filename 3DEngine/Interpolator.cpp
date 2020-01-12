#include "Interpolator.h"

Interpolator::Interpolator (Vertex* left, Vertex* right, Vertex* other, TriangleType tt) {

	Vec4* leftPosition = &left->renderPosition;
	Vec4* rightPosition = &right->renderPosition;
	Vec4* otherPosition = &other->renderPosition;

	Vec2* leftTexel = &left->texel;
	Vec2* rightTexel = &right->texel;
	Vec2* otherTexel = &other->texel;

	Vec4* leftNormal = &left->normal;
	Vec4* rightNormal = &right->normal;
	Vec4* otherNormal = &other->normal;

	Vec4* leftWorld = &left->worldPosition;
	Vec4* rightWorld = &right->worldPosition;
	Vec4* otherWorld = &other->worldPosition;

	double deltaY = 0;
	if (tt == TriangleType::FlatBottom) {
		//other means top

		deltaY = leftPosition->y - otherPosition->y;

		/*x(otherPosition->x, leftPosition->x, otherPosition->x, rightPosition->x, deltaY);

		z(otherPosition->z, leftPosition->z, otherPosition->z, rightPosition->z, deltaY);
		inverseW(1.0 / otherPosition->w, 1.0 / leftPosition->w, 1.0 / otherPosition->w, 1.0 / rightPosition->w, deltaY);

		uPersp(otherTexel->x / otherPosition->w, leftTexel->x / leftPosition->w,
			otherTexel->x / otherPosition->w, rightTexel->x / rightPosition->w, deltaY);

		vPersp(otherTexel->y / otherPosition->w, leftTexel->y / leftPosition->w,
			otherTexel->y / otherPosition->w, rightTexel->y / rightPosition->w, deltaY);

		nx(otherNormal->x, leftNormal->x, otherNormal->x, rightNormal->x, deltaY);
		ny(otherNormal->y, leftNormal->y, otherNormal->y, rightNormal->y, deltaY);
		nz(otherNormal->z, leftNormal->z, otherNormal->z, rightNormal->z, deltaY);*/

		YTop = otherPosition->y;
		YBottom = leftPosition->y;

	}
	else if (tt == TriangleType::FlatTop) {

		//other means bottom
		deltaY = otherPosition->y - leftPosition->y;

		/*x(leftPosition->x, otherPosition->x, rightPosition->x, otherPosition->x, deltaY);

		z(leftPosition->z, otherPosition->z, rightPosition->z, otherPosition->z, deltaY);
		inverseW(1.0 / leftPosition->w, 1.0 / otherPosition->w, 1.0 / rightPosition->w,  1.0 / otherPosition->w, deltaY);

		uPersp(leftTexel->x / leftPosition->w, otherTexel->x / otherPosition->w,
			   rightTexel->x / rightPosition->w, otherTexel->x / otherPosition->w, deltaY);

		vPersp(leftTexel->y / leftPosition->w, otherTexel->y / otherPosition->w,
			   rightTexel->y / rightPosition->w, otherTexel->y / otherPosition->w,  deltaY);

		nx(leftNormal->x, otherNormal->x, rightNormal->x, otherNormal->x, deltaY);
		ny(leftNormal->y, otherNormal->y, rightNormal->y, otherNormal->y, deltaY);
		nz(leftNormal->z, otherNormal->z, rightNormal->z, otherNormal->z, deltaY);*/

		YTop = leftPosition->y;
		YBottom = otherPosition->y;
	}

	MaxX = rightPosition->x > otherPosition->x ? rightPosition->x : otherPosition->x;

}

void Interpolator::MoveDown() {

	/*x.MoveDown(1);

	double deltaX = x.right - x.left;

	z.MoveDown(deltaX);

	inverseW.MoveDown(deltaX);

	uPersp.MoveDown(deltaX);
	vPersp.MoveDown(deltaX);

	nx.MoveDown(deltaX);
	ny.MoveDown(deltaX);
	nz.MoveDown(deltaX);

	X1 = x.left;
	X2 = x.right;*/
}

void Interpolator::MoveAcross() {

	/*z.MoveAcross();

	inverseW.MoveAcross();

	uPersp.MoveAcross();
	vPersp.MoveAcross();

	nx.MoveAcross();
	ny.MoveAcross();
	nz.MoveAcross();

	Z_NDC = z.across;

	double w = 1.0 / inverseW.across;
	Texel.x = uPersp.across * w;
	Texel.y = vPersp.across * w;

	Normal.x = nx.across;
	Normal.y = ny.across;
	Normal.z = nz.across;*/
	
}
