#include "VIS.h"
#include "ShapeDiameterFunction.h"

std::vector<SDFUnit> VIS::computeMeshVIS(int rayCount) {
	std::vector<SDFUnit> rays;
	return rays;
};

std::vector<SDFUnit> VIS::computeFaceVIS(face_iterator face, int rayCount) {
	 std::vector<SDFUnit> rays = compute(face, 60, 30);
	 SDFUnit maxUnit;
	 double maxLine = -9999999;
	 for (SDFUnit &unit : rays) {
		 if (unit.getLineLength() > maxLine) {
			 maxLine = unit.getLineLength();
			 maxUnit = unit; 
		 }
	 }
	 Kernel::Point_3 midPoint = CGAL::midpoint(maxUnit.getEndPoint(), maxUnit.getStartPoint());
	 std::vector<Kernel::Vector_3> rays2 = buildRays(glm::vec3(0.0,1.0,0.0), rayCount, 180);
	 std::vector<SDFUnit> rays3 = computeIntersection(midPoint, Kernel::Vector_3(0.0, 1.0, 0.0), rays2);
	 maxUnit.setColor(glm::vec3(0, 0, 1.0));
	 rays3.push_back(maxUnit);
	 return rays3;
}