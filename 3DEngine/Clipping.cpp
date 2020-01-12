#include "Renderer.h"

void Renderer::CullAndClip(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity) {

	//cull against all sides of NDC space
	//far
	if (vertex1.renderPosition.z > vertex1.renderPosition.w * FAR_CLIP_PLANE 
	 && vertex2.renderPosition.z > vertex2.renderPosition.w * FAR_CLIP_PLANE 
	 && vertex3.renderPosition.z > vertex3.renderPosition.w * FAR_CLIP_PLANE) {
		return;
	}
	//near
	if (vertex1.renderPosition.z < vertex1.renderPosition.w * NEAR_CLIP_PLANE
	 && vertex2.renderPosition.z < vertex2.renderPosition.w * NEAR_CLIP_PLANE
	 && vertex3.renderPosition.z < vertex3.renderPosition.w * NEAR_CLIP_PLANE) {
		return;
	}
	//left
	if (vertex1.renderPosition.x < vertex1.renderPosition.w * LEFT_CLIP_PLANE
	 && vertex2.renderPosition.x < vertex2.renderPosition.w * LEFT_CLIP_PLANE
	 && vertex3.renderPosition.x < vertex3.renderPosition.w * LEFT_CLIP_PLANE) {
		return;
	}
	//right
	if (vertex1.renderPosition.x > vertex1.renderPosition.w * RIGHT_CLIP_PLANE
	 && vertex2.renderPosition.x > vertex2.renderPosition.w * RIGHT_CLIP_PLANE
	 && vertex3.renderPosition.x > vertex3.renderPosition.w * RIGHT_CLIP_PLANE) {
		return;
	}
	//top
	if (vertex1.renderPosition.y > vertex1.renderPosition.w * TOP_CLIP_PLANE
	 && vertex2.renderPosition.y > vertex2.renderPosition.w * TOP_CLIP_PLANE
	 && vertex3.renderPosition.y > vertex3.renderPosition.w * TOP_CLIP_PLANE) {
		return;
	}
	//bottom
	if (vertex1.renderPosition.y < vertex1.renderPosition.w * BOTTOM_CLIP_PLANE
	 && vertex2.renderPosition.y < vertex2.renderPosition.w * BOTTOM_CLIP_PLANE
	 && vertex3.renderPosition.y < vertex3.renderPosition.w * BOTTOM_CLIP_PLANE) {
		return;
	}

	//clipping chain for all 6 faces
	//Near -> Far -> Left -> Right -> Bottom -> Top
	
	(this->*clipChain[0])(vertex1, vertex2, vertex3, face, entity, 0);

}

void Renderer::Clip1(Vertex& outside, Vertex& inside1, Vertex& inside2, Face* face, Entity* entity, int planeID) {

	Vec4& plane = planeNormals[planeID];

	double dist1 = outside.renderPosition * plane + outside.renderPosition.w;
	double dist2 = inside1.renderPosition * plane + inside1.renderPosition.w;

	if (dist1 * dist2 > 0) {
		//std::cout << "Clipping Error!" << std::endl;
		return;
	}

	double alpha = dist1 / (dist1 - dist2);

	Vertex onLine1 = outside * (1 - alpha) + inside1 * alpha;

	dist2 = inside2.renderPosition * plane + inside2.renderPosition.w;

	if (dist1 * dist2 > 0) {
		//std::cout << "Clipping Error!" << std::endl;
		return;
	}

	alpha = dist1 / (dist1 - dist2);

	Vertex onLine2 = outside * (1 - alpha) + inside2 * alpha;

	(this->*clipChain[planeID + 1])(onLine2, inside1, onLine1, face, entity, planeID + 1);
	(this->*clipChain[planeID + 1])(onLine2, inside2, inside1, face, entity, planeID + 1);

}

void Renderer::Clip2(Vertex& outside1, Vertex& outside2, Vertex& inside, Face* face, Entity* entity, int planeID) {
	
	Vec4& plane = planeNormals[planeID];

	double dist1 = outside1.renderPosition * plane + outside1.renderPosition.w;
	double dist2 = inside.renderPosition * plane + inside.renderPosition.w;

	if (dist1 * dist2 > 0) {
		//std::cout << "Clipping Error!" << std::endl;
		//std::cout << dist1 << " " << dist2 << std::endl;
		return;
	}

	double alpha = dist1 / (dist1 - dist2);

	Vertex onLine1 = outside1 * (1 - alpha) + inside * alpha;

	dist1 = outside2.renderPosition * plane + outside2.renderPosition.w;

	if (dist1 * dist2 > 0) {
		//std::cout << "Clipping Error!" << std::endl;
		//std::cout << dist1 << " " << dist2 << std::endl;
		return;
	}

	alpha = dist1 / (dist1 - dist2);

	Vertex onLine2 = outside2 * (1 - alpha) + inside * alpha;

	(this->*clipChain[planeID + 1])(inside, onLine1, onLine2, face, entity, planeID + 1);

}

void Renderer::FinishClipping(Vertex& v1, Vertex& v2, Vertex& v3, Face* face, Entity* entity, int planeID) {
	FillTriangle(v1, v2, v3, face, entity);
}

void Renderer::ClipNear(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity, int planeID) {

	if (vertex1.renderPosition.z < vertex1.renderPosition.w * NEAR_CLIP_PLANE) {
		if (vertex2.renderPosition.z < vertex2.renderPosition.w * NEAR_CLIP_PLANE) {
			Clip2(vertex1, vertex2, vertex3, face, entity, planeID);
		}
		else if (vertex3.renderPosition.z < vertex3.renderPosition.w * NEAR_CLIP_PLANE) {
			Clip2(vertex1, vertex3, vertex2, face, entity, planeID);
		}
		else {
			Clip1(vertex1, vertex2, vertex3, face, entity, planeID);
		}
	}
	else if (vertex2.renderPosition.z < vertex2.renderPosition.w * NEAR_CLIP_PLANE) {
		if (vertex3.renderPosition.z < vertex3.renderPosition.w * NEAR_CLIP_PLANE) {
			Clip2(vertex2, vertex3, vertex1, face, entity, planeID);
		}
		else {
			Clip1(vertex2, vertex1, vertex3, face, entity, planeID);
		}
	}
	else if (vertex3.renderPosition.z < vertex3.renderPosition.w * NEAR_CLIP_PLANE) {
		Clip1(vertex3, vertex1, vertex2, face, entity, planeID);
	}
	else {

		(this->*clipChain[planeID + 1])(vertex1, vertex2, vertex3, face, entity, planeID + 1);
	}

}

void Renderer::ClipFar(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity, int planeID) {

	if (vertex1.renderPosition.z > vertex1.renderPosition.w * FAR_CLIP_PLANE) {
		if (vertex2.renderPosition.z > vertex2.renderPosition.w * FAR_CLIP_PLANE) {
			Clip2(vertex1, vertex2, vertex3, face, entity, planeID);
		}
		else if (vertex3.renderPosition.z > vertex3.renderPosition.w * FAR_CLIP_PLANE) {
			Clip2(vertex1, vertex3, vertex2, face, entity, planeID);
		}
		else {
			Clip1(vertex1, vertex2, vertex3, face, entity, planeID);
		}
	}
	else if (vertex2.renderPosition.z > vertex2.renderPosition.w * FAR_CLIP_PLANE) {
		if (vertex3.renderPosition.z > vertex3.renderPosition.w * FAR_CLIP_PLANE) {
			Clip2(vertex2, vertex3, vertex1, face, entity, planeID);
		}
		else {
			Clip1(vertex2, vertex1, vertex3, face, entity, planeID);
		}
	}
	else if (vertex3.renderPosition.z > vertex3.renderPosition.w * FAR_CLIP_PLANE) {
		Clip1(vertex3, vertex1, vertex2, face, entity, planeID);
	}
	else {

		(this->*clipChain[planeID + 1])(vertex1, vertex2, vertex3, face, entity, planeID + 1);
	}
}

void Renderer::ClipLeft(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity, int planeID) {

	if (vertex1.renderPosition.x < vertex1.renderPosition.w * LEFT_CLIP_PLANE) {
		if (vertex2.renderPosition.x < vertex2.renderPosition.w * LEFT_CLIP_PLANE) {
			Clip2(vertex1, vertex2, vertex3, face, entity, planeID);
		}
		else if (vertex3.renderPosition.x < vertex3.renderPosition.w * LEFT_CLIP_PLANE) {
			Clip2(vertex1, vertex3, vertex2, face, entity, planeID);
		}
		else {
			Clip1(vertex1, vertex2, vertex3, face, entity, planeID);
		}
	}
	else if (vertex2.renderPosition.x < vertex2.renderPosition.w * LEFT_CLIP_PLANE) {
		if (vertex3.renderPosition.x < vertex3.renderPosition.w * LEFT_CLIP_PLANE) {
			Clip2(vertex2, vertex3, vertex1, face, entity, planeID);
		}
		else {
			Clip1(vertex2, vertex1, vertex3, face, entity, planeID);
		}
	}
	else if (vertex3.renderPosition.x < vertex3.renderPosition.w * LEFT_CLIP_PLANE) {
		Clip1(vertex3, vertex1, vertex2, face, entity, planeID);
	}
	else {

		(this->*clipChain[planeID + 1])(vertex1, vertex2, vertex3, face, entity, planeID + 1);
	}

}

void Renderer::ClipRight(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity, int planeID) {

	if (vertex1.renderPosition.x > vertex1.renderPosition.w * RIGHT_CLIP_PLANE) {
		if (vertex2.renderPosition.x > vertex2.renderPosition.w * RIGHT_CLIP_PLANE) {
			Clip2(vertex1, vertex2, vertex3, face, entity, planeID);
		}
		else if (vertex3.renderPosition.x > vertex3.renderPosition.w * RIGHT_CLIP_PLANE) {
			Clip2(vertex1, vertex3, vertex2, face, entity, planeID);
		}
		else {
			Clip1(vertex1, vertex2, vertex3, face, entity, planeID);
		}
	}
	else if (vertex2.renderPosition.x > vertex2.renderPosition.w * RIGHT_CLIP_PLANE) {
		if (vertex3.renderPosition.x > vertex3.renderPosition.w * RIGHT_CLIP_PLANE) {
			Clip2(vertex2, vertex3, vertex1, face, entity, planeID);
		}
		else {
			Clip1(vertex2, vertex1, vertex3, face, entity, planeID);
		}
	}
	else if (vertex3.renderPosition.x > vertex3.renderPosition.w * RIGHT_CLIP_PLANE) {
		Clip1(vertex3, vertex1, vertex2, face, entity, planeID);
	}
	else {

		(this->*clipChain[planeID + 1])(vertex1, vertex2, vertex3, face, entity, planeID + 1);
	}
}

void Renderer::ClipBottom(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity, int planeID) {

	if (vertex1.renderPosition.y < vertex1.renderPosition.w * BOTTOM_CLIP_PLANE) {
		if (vertex2.renderPosition.y < vertex2.renderPosition.w * BOTTOM_CLIP_PLANE) {
			Clip2(vertex1, vertex2, vertex3, face, entity, planeID);
		}
		else if (vertex3.renderPosition.y < vertex3.renderPosition.w * BOTTOM_CLIP_PLANE) {
			Clip2(vertex1, vertex3, vertex2, face, entity, planeID);
		}
		else {
			Clip1(vertex1, vertex2, vertex3, face, entity, planeID);
		}
	}
	else if (vertex2.renderPosition.y < vertex2.renderPosition.w * BOTTOM_CLIP_PLANE) {
		if (vertex3.renderPosition.y < vertex3.renderPosition.w * BOTTOM_CLIP_PLANE) {
			Clip2(vertex2, vertex3, vertex1, face, entity, planeID);
		}
		else {
			Clip1(vertex2, vertex1, vertex3, face, entity, planeID);
		}
	}
	else if (vertex3.renderPosition.y < vertex3.renderPosition.w * BOTTOM_CLIP_PLANE) {
		Clip1(vertex3, vertex1, vertex2, face, entity, planeID);
	}
	else {

		(this->*clipChain[planeID + 1])(vertex1, vertex2, vertex3, face, entity, planeID + 1);
	}
}

void Renderer::ClipTop(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, Face* face, Entity* entity, int planeID) {

	if (vertex1.renderPosition.y > vertex1.renderPosition.w * TOP_CLIP_PLANE) {
		if (vertex2.renderPosition.y > vertex2.renderPosition.w * TOP_CLIP_PLANE) {
			Clip2(vertex1, vertex2, vertex3, face, entity, planeID);
		}
		else if (vertex3.renderPosition.y > vertex3.renderPosition.w * TOP_CLIP_PLANE) {
			Clip2(vertex1, vertex3, vertex2, face, entity, planeID);
		}
		else {
			Clip1(vertex1, vertex2, vertex3, face, entity, planeID);
		}
	}
	else if (vertex2.renderPosition.y > vertex2.renderPosition.w * TOP_CLIP_PLANE) {
		if (vertex3.renderPosition.y > vertex3.renderPosition.w * TOP_CLIP_PLANE) {
			Clip2(vertex2, vertex3, vertex1, face, entity, planeID);
		}
		else {
			Clip1(vertex2, vertex1, vertex3, face, entity, planeID);
		}
	}
	else if (vertex3.renderPosition.y > vertex3.renderPosition.w * TOP_CLIP_PLANE) {
		Clip1(vertex3, vertex1, vertex2, face, entity, planeID);
	}
	else {

		(this->*clipChain[planeID + 1])(vertex1, vertex2, vertex3, face, entity, planeID + 1);
	}
}