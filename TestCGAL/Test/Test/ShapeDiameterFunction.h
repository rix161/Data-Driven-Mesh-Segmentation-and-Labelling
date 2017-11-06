#ifndef __SHAPE_DIAMETER_FUNCTION__
#define __SHAPE_DIAMETER_FUNCTION__
#include "CGAL_geodesic.h"
#include "SDFUnit.h"
#include <vector>
#include <thread>
#include <future>
#include <mutex> 

class ShapeDiameterFunction{

protected:

	bool doesInterset(
		Kernel::Point_3 source,
		Kernel::Vector_3 direction,
		std::vector<Kernel::Point_3> triangleFaces,
		Kernel::Point_3 &intersectionPoint);
	std::vector<Kernel::Vector_3> buildRays(glm::vec3 normal, int count, float angle);

	void optidoesIntersect(
		face_iterator triangleFace,
		Kernel::Point_3 source,
		Kernel::Vector_3 rays,
		std::vector<Kernel::Point_3> triangleFaces,
		std::vector<SDFUnit> &sdfUnits);

	std::vector<SDFUnit> computeIntersection(std::vector<Kernel::Vector_3> rays);
	std::mutex mMutex;

public:
	std::vector<SDFUnit> compute(Triangle_mesh mesh, face_iterator face, float coneAngle, int rayCount);
};


#endif