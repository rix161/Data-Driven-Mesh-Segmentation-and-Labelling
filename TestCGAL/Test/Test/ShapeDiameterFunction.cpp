#include "ShapeDiameterFunction.h"
#include <glm/gtx/rotate_vector.hpp>


bool ShapeDiameterFunction::doesInterset(
	Kernel::Point_3 source,
	Kernel::Vector_3 direction,
	face_iterator intersectTriangle,
	Triangle_mesh mesh,
	Kernel::Point_3 &intersectionPoint) {
	//Implementation of Möller–Trumbore intersection algorithm

	std::vector<Kernel::Point_3> triangleVertices;
	Kernel::Vector_3 rayDirection = direction;
	double threshold = 0.0000001;

	CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
	for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mesh.halfedge(*intersectTriangle), mesh);
		vbegin != vend; vbegin++) {
		triangleVertices.push_back(mesh.point((*vbegin)));
	}
	
	Kernel::Vector_3 vecBA = triangleVertices[1] - triangleVertices[0];
	Kernel::Vector_3 vecCA = triangleVertices[2] - triangleVertices[0];
	Kernel::Vector_3 vecP = CGAL::cross_product(rayDirection, vecCA);
	double det = vecBA * vecP;
	if (det < threshold)
		return false;

	double invDet = 1 / det;

	Kernel::Vector_3 vecT = source - triangleVertices[0];
	double parameterU = vecT*vecP*invDet;
	if (parameterU < 0 || parameterU >1) return false;

	Kernel::Vector_3 vecQ = CGAL::cross_product(vecT, vecBA);
	double parameterV = rayDirection*vecQ*invDet;
	if (parameterV < 0 || parameterU>1) return false;
	
	//compute Intersection Point
	double parameterT = vecCA*vecQ;
	if (parameterT > threshold) {
		intersectionPoint = source + parameterT*rayDirection;
		return true;
	}

	return false;
}


//Taken from https://stackoverflow.com/questions/4310277/producing-random-float-from-negative-to-positive-range
float RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}


std::vector<Kernel::Vector_3> ShapeDiameterFunction::buildRays(glm::vec3 normal, int count, float angle) {
	std::vector<Kernel::Vector_3> rays;
	for (int i = 0; i < count; i++) {
		glm::vec3 temp = glm::rotate(normal, RandomNumber(-angle, angle), normal);
		Kernel::Vector_3 ray(temp[0], temp[1], temp[2]);
		rays.push_back(ray);
	}
	return rays;
}

std::vector<SDFUnit> ShapeDiameterFunction::compute(Triangle_mesh mesh, face_iterator face, float coneAngle, int rayCount) {

	std::vector<Kernel::Point_3> faceVertices;
	CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
	std::vector<SDFUnit> sdfData;
	glm::vec3 lineColor(1.0, 0.0, 0.0);

	for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mesh.halfedge(*face), mesh);
		vbegin != vend; vbegin++) {
		faceVertices.push_back(mesh.point((*vbegin)));
	}

	if (faceVertices.size() != 3) return sdfData;

	Kernel::Vector_3 faceNormal = CGAL::normal(faceVertices[0], faceVertices[1], faceVertices[2]);
	Kernel::Point_3 faceCenter = CGAL::centroid(faceVertices[0], faceVertices[1], faceVertices[2]);
	std::cout << "FaceNormal:" << faceNormal << "FaceCenter" << faceCenter << std::endl;
	glm::vec3 rotVec = glm::vec3(faceNormal.x(), faceNormal.y(), faceNormal.z());
	std::vector<Kernel::Vector_3> rays = buildRays(rotVec, rayCount, coneAngle);


	for (face_iterator fIter = mesh.faces_begin(); fIter != mesh.faces_end(); fIter++) {
		for (Kernel::Vector_3 ray : rays) {
			Kernel::Point_3 interPoint(0, 0, 0);
			if (doesInterset(faceCenter, ray, fIter,mesh,interPoint)) {
				sdfData.push_back(SDFUnit(fIter, faceCenter, interPoint, lineColor));
			}
		}
	}

	return sdfData;
}
