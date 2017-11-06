#ifndef __SHAPE_DIAMETER_FUNCTION__
#define __SHAPE_DIAMETER_FUNCTION__
#include "CGAL_geodesic.h"
#include "SDFUnit.h"
#include "GenericMesh.h"
#include <vector>
#include <iostream>
#include <fstream>

class ShapeDiameterFunction:public GenericMesh{

protected:

	bool doesInterset(
		Kernel::Point_3 source,
		Kernel::Vector_3 direction,
		std::vector<Kernel::Point_3> triangleFaces,
		Kernel::Point_3 &intersectionPoint,
		bool bypass=false);
	std::vector<Kernel::Vector_3> buildRays(glm::vec3 normal, int count, float angle,bool bypass=false);

	void optidoesIntersect(
		face_iterator triangleFace,
		Kernel::Point_3 source,
		Kernel::Vector_3 rays,
		std::vector<Kernel::Point_3> triangleFaces,
		std::vector<SDFUnit> &sdfUnits);

	std::vector<SDFUnit> computeIntersection(Kernel::Point_3 faceCenter, Kernel::Vector_3 faceNormal, std::vector<Kernel::Vector_3> rays,bool bypass=false);

public:

	ShapeDiameterFunction(const char* fileName) {
		std::ifstream in(fileName);
		in >> this->mMainMesh;
		std::cout << mMainMesh.number_of_vertices() << std::endl;
		in.close();
	}

	std::vector<SDFUnit> compute(face_iterator face, float coneAngle, int rayCount);
};


#endif