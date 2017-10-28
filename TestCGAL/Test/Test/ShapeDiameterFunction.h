#ifndef __SHAPE_DIAMETER_FUNCTION__
#define __SHAPE_DIAMETER_FUNCTION__
#include "CGAL_geodesic.h"
#include "SDFUnit.h"
#include <vector>
class ShapeDiameterFunction{

private:

	bool doesInterset(
		Kernel::Point_3 source,
		Kernel::Vector_3 direction,
		face_iterator intersectTriangle,
		Triangle_mesh mesh,
		Kernel::Point_3 &intersectionPoint);
	std::vector<Kernel::Vector_3> buildRays(glm::vec3 normal, int count, float angle);
public:
	std::vector<SDFUnit> compute(Triangle_mesh mesh, face_iterator face, float coneAngle, int rayCount);
};


#endif