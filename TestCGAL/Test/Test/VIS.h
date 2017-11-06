#ifndef __VIS__
#define __VIS__
#include "ShapeDiameterFunction.h"
class VIS :public ShapeDiameterFunction {
public:
	std::vector<SDFUnit> computeMeshVIS(Triangle_mesh mesh, float coneAngle, int rayCount);
	std::vector<SDFUnit> computeFaceVIS(Triangle_mesh mesh, face_iterator face, float coneAngle, int rayCount);

};

#endif