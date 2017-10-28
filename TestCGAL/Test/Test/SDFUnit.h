#ifndef __SDF_UNIT__
#define __SDF_UNIT__
#include "CGAL_geodesic.h"
#include <glm/glm.hpp>

class SDFUnit {
private:
	face_iterator mFaceIter;
	Kernel::Point_3 mStartPoint;
	Kernel::Point_3 mEndPoint;
	glm::vec3 mLineColor;

public:
	SDFUnit(face_iterator faceIter, Kernel::Point_3 startPoint, Kernel::Point_3 endPoint, glm::vec3 color) {
		this->mFaceIter = faceIter;
		this->mStartPoint = startPoint;
		this->mEndPoint = endPoint;
		this->mLineColor = color;
	}

	face_iterator getFaceIterator() { return mFaceIter; }
	Kernel::Point_3 getStartPoint() { return mStartPoint; }
	Kernel::Point_3 getEndPoint() { return mEndPoint;}
	glm::vec3 getColor() { return mLineColor; }
};

#endif
