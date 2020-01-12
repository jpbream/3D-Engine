#pragma once

struct Attribute {

private:
	double slopeLeft;
	double slopeRight;

	double slopeAcross;

public:

	Attribute() {}

	void operator()(double leftSideTop, double leftSideBottom, double rightSideTop, double rightSideBottom, double deltaY) {

		slopeLeft = (leftSideBottom - leftSideTop) / deltaY;
		slopeRight = (rightSideBottom - rightSideTop) / deltaY;

		right = rightSideTop - slopeRight;
		left = leftSideTop - slopeLeft;
		across = left;

	}

	double right;
	double left;
	double across;

	inline void MoveDown(double rowDeltaX) {

		right += slopeRight;
		left += slopeLeft;

		slopeAcross = (right - left) / rowDeltaX;
		across = left - slopeAcross;

	}
	inline void MoveAcross() {

		across += slopeAcross;

	}

};