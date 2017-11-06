#ifndef __VIS__
#define __VIS__
#include "ShapeDiameterFunction.h"
class VIS :public ShapeDiameterFunction {
public:
	VIS(const char* fileName):ShapeDiameterFunction(fileName) {}
	std::vector<SDFUnit> computeMeshVIS(int rayCount);
	std::vector<SDFUnit> computeFaceVIS(face_iterator face,int rayCount);
};

#endif