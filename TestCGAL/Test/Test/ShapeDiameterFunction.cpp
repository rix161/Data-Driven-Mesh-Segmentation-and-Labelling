#include "ShapeDiameterFunction.h"
#include <glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>


void ShapeDiameterFunction::optidoesIntersect(
	face_iterator triangleFace,
	Kernel::Point_3 source,
	Kernel::Vector_3 rayDirection,
	std::vector<Kernel::Point_3> triangleVertices,
	std::vector<SDFUnit> &sdfUnits) {

	double threshold = 1e-8;

	//std::cout << "Vec1:" << triangleVertices[0] << " Vec2:" << triangleVertices[1] << " Vec3:" << triangleVertices[2]<<std::endl;
	Kernel::Vector_3 vecBA = triangleVertices[1] - triangleVertices[0];
	Kernel::Vector_3 vecCA = triangleVertices[2] - triangleVertices[0];
	Kernel::Vector_3 vecP = CGAL::cross_product(rayDirection, vecCA);
	double det = vecBA * vecP;
	if (det > threshold && det < threshold)
		return;

	double invDet = 1 / det;

	Kernel::Vector_3 vecT = source - triangleVertices[0];
	double parameterU = vecT*vecP*invDet;
	if (parameterU < 0 || parameterU >1) return;

	Kernel::Vector_3 vecQ = CGAL::cross_product(vecT, vecBA);
	double parameterV = rayDirection*vecQ*invDet;
	if (parameterV < 0 || parameterV>1) return;

	//compute Intersection Point
	double parameterT = vecCA*vecQ*invDet;
	if (parameterT > threshold) {
		sdfUnits.push_back(SDFUnit(triangleFace, source, source + parameterT*rayDirection, glm::vec3(1.0, 0.0, 0.0)));
		return;
	}

	return;
}

bool ShapeDiameterFunction::doesInterset(
	Kernel::Point_3 source,
	Kernel::Vector_3 rayDirection,
	std::vector<Kernel::Point_3> triangleVertices,
	Kernel::Point_3 &intersectionPoint,
	bool bypass) {
	//Implementation of Möller–Trumbore intersection algorithm

	double threshold = 1e-8;

	//std::cout << "Vec1:" << triangleVertices[0] << " Vec2:" << triangleVertices[1] << " Vec3:" << triangleVertices[2]<<std::endl;
	Kernel::Vector_3 vecBA = triangleVertices[1] - triangleVertices[0];
	Kernel::Vector_3 vecCA = triangleVertices[2] - triangleVertices[0];
	Kernel::Vector_3 vecP = CGAL::cross_product(rayDirection, vecCA);
	double det = vecBA * vecP;
	if (!bypass && (det > threshold && det < threshold))
		return false;

	double invDet = 1 / det;

	Kernel::Vector_3 vecT = source - triangleVertices[0];
	double parameterU = vecT*vecP*invDet;
	if (parameterU < 0 || parameterU >1) return false;

	Kernel::Vector_3 vecQ = CGAL::cross_product(vecT, vecBA);
	double parameterV = rayDirection*vecQ*invDet;
	if (parameterV < 0 || parameterU+parameterV>1) return false;
	
	//compute Intersection Point
	double parameterT = vecCA*vecQ*invDet;
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


std::vector<Kernel::Vector_3> ShapeDiameterFunction::buildRays(glm::vec3 normal, int count, float angle,bool bypass) {
	std::vector<Kernel::Vector_3> rays;
	
	Kernel::Vector_3 normal2(normal[0], normal[1], normal[2]);
	rays.push_back(normal2);
	for (int i = 0; i < count; i++) {
		float degAngle = RandomNumber(-angle, angle);
		if(bypass)
			normal = degAngle < 0 ? normal*-1.0f:normal;
		//std::cout << "DegAngle:" << degAngle<<std::endl;
		glm::vec3 temp = glm::rotate(normal, glm::radians(degAngle), glm::vec3(RandomNumber(0,1), RandomNumber(0, 1), RandomNumber(0, 1)));
		/*glm::vec3 temp2 = glm::rotate(normal, degAngle, glm::vec3(1, 0, 0));
		std::cout << "Angle B/w vectors1:" << glm::degrees(glm::angle(temp, normal)) << std::endl;
		std::cout << "Angle B/w vectors2:" << glm::angle(temp2, normal)<< std::endl;*/
		Kernel::Vector_3 ray(temp[0], temp[1], temp[2]);
		/*Kernel::Vector_3 ray2(temp2[0], temp2[1], temp2[2]);
		std::cout << "CGAL Angle B/w vectors1:" <<CGAL::angle(ray,normal2) << std::endl;
		std::cout << "CGAL Angle B/w vectors2:" << CGAL::angle(ray2, normal2) << std::endl;*/
		rays.push_back(ray);
	}
	return rays;
}

std::vector<SDFUnit> ShapeDiameterFunction::computeIntersection(Kernel::Point_3 faceCenter,Kernel::Vector_3 faceNormal,std::vector<Kernel::Vector_3> rays,bool bypass) {

	std::vector<SDFUnit> sdfData;
	
	for (Kernel::Vector_3 ray : rays) {
		double minRayLen = 99999;
		Kernel::Point_3 minInterPoint;
		face_iterator minIter;

		for (face_iterator fIter = mMainMesh.faces_begin(); fIter != mMainMesh.faces_end(); fIter++) {
			std::vector<Kernel::Point_3> triangleVertices;
			Kernel::Point_3 interPoint(0, 0, 0);
			glm::vec3 lineColor(0.0, 1.0, 0.0);

			CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
			for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*fIter), mMainMesh);
				vbegin != vend; vbegin++) {
				triangleVertices.push_back(mMainMesh.point((*vbegin)));
			}
			
			if (doesInterset(faceCenter, ray, triangleVertices, interPoint)) {
				if (CGAL::squared_distance(faceCenter, interPoint) < minRayLen) {
					minInterPoint = interPoint;
					minRayLen = CGAL::squared_distance(faceCenter, interPoint);
					minIter = fIter;
				}
			}
		}
		sdfData.push_back(SDFUnit(minIter, faceCenter, minInterPoint, glm::vec3(0, 1, 0)));
	}
	return sdfData;

}

std::vector<SDFUnit> ShapeDiameterFunction::compute(face_iterator face, float coneAngle, int rayCount) {

	std::vector<Kernel::Point_3> faceVertices;
	CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
	glm::vec3 lineColor(0.0, 1.0, 0.0);
	std::vector<SDFUnit> sdfData;

	for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*face), mMainMesh);
		vbegin != vend; vbegin++) {
		faceVertices.push_back(mMainMesh.point((*vbegin)));
	}

	if (faceVertices.size() != 3) return sdfData;

	Kernel::Vector_3 faceNormal = CGAL::normal(faceVertices[0], faceVertices[1], faceVertices[2]);
	faceNormal = faceNormal*-1;
	Kernel::Point_3 faceCenter = CGAL::centroid(faceVertices[0], faceVertices[1], faceVertices[2]);
	glm::vec3 rotVec = glm::vec3(faceNormal.x(), faceNormal.y(), faceNormal.z());
	std::vector<Kernel::Vector_3> rays = buildRays(rotVec, rayCount, coneAngle);

	return computeIntersection(faceCenter, faceNormal,rays);
}
