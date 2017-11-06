#include "VIS.h"
#include "ShapeDiameterFunction.h"

std::vector<SDFUnit> VIS::computeMeshVIS(Triangle_mesh mesh, float coneAngle, int rayCount) {
	std::vector<SDFUnit> rays;
	return rays;
};

std::vector<SDFUnit> VIS::computeFaceVIS(Triangle_mesh mesh, face_iterator face, float coneAngle, int rayCount) {
	 std::vector<SDFUnit> rays = compute(mesh, face, coneAngle, rayCount);
	 SDFUnit maxUnit;
	 double maxLine = -9999999;
	 for (SDFUnit unit : rays) {
		 if (unit.getLineLength() > maxLine) maxUnit = unit;
	 }
	 Kernel::Point_3 midPoint = CGAL::midpoint(maxUnit.getEndPoint(), maxUnit.getStartPoint());

	 //std::vector<SDFUnit> rays2 = compute(mesh, midPoint, 180, rayCount);
	 return rays;
}