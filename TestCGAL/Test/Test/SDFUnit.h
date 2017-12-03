#ifndef __SDF_UNIT__
#define __SDF_UNIT__
#include <glm/glm.hpp>
#include "GenericMesh.h"
class SDFUnit {
private:
	face_iterator mFaceIter;
	Kernel::Point_3 mStartPoint;
	Kernel::Point_3 mEndPoint;
	glm::vec3 mLineColor;
	float angle;

public:
	SDFUnit() {}
	SDFUnit(face_iterator faceIter, Kernel::Point_3 startPoint, Kernel::Point_3 endPoint, glm::vec3 color,float angle) {
		this->mFaceIter = faceIter;
		this->mStartPoint = startPoint;
		this->mEndPoint = endPoint;
		this->mLineColor = color;
		this->angle = angle;
	}

	face_iterator getFaceIterator() { return mFaceIter; }
	Kernel::Point_3 getStartPoint() { return mStartPoint; }
	Kernel::Point_3 getEndPoint() { return mEndPoint;}
	double getLineLength() { return sqrt((mEndPoint - mStartPoint).squared_length()); }
	double getWeightedLineLength() { return sqrt((mEndPoint - mStartPoint).squared_length()); }

	glm::vec3 getColor() { return mLineColor; }
	void setColor(glm::vec3 color) { this->mLineColor = color; }
};

#endif
