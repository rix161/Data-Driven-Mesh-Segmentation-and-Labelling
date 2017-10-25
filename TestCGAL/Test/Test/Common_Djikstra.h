#ifndef __COMMON_DIJKSTRA__
#define __COMMON_DIJKSTRA__

#include "CGAL_geodesic.h"
#include <vector>

class DjiElement {

private:
	vertex_iterator mVertex;
	Kernel::Point_3 mPoint;
	double mDistance;
public:
	DjiElement(vertex_iterator vIter, Kernel::Point_3 point, double distance) {
		this->mVertex = vIter;
		this->mPoint = point;
		this->mDistance = distance;
	}

	vertex_iterator getVertex() const { return this->mVertex; }
	Kernel::Point_3 getPoint() const { return this->mPoint; }
	double getDistance() const { return this->mDistance; }
	void updateDistance(double newDistance) { this->mDistance = newDistance; }
};


struct compare {
	bool operator()(const DjiElement& left, const DjiElement& right)const {
		return left.getDistance() > right.getDistance();
	}
};

struct setCompare {
	bool operator()(const DjiElement& left, const DjiElement& right)const {
		Kernel::Point_3 lPoint, rPoint;
		lPoint = left.getPoint();
		rPoint = right.getPoint();
		if ((lPoint.x() == rPoint.x()) && (lPoint.y() == rPoint.y()))
			return true;
		else
			return false;
	}
};

#endif