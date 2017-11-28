#ifndef __VIS__
#define __VIS__
#include "ShapeDiameterFunction.h"
class VIS :public ShapeDiameterFunction {
public:
	VIS(const char* fileName):ShapeDiameterFunction(fileName) {}
	std::vector<SDFUnit> computeMeshVIS(int rayCount);
	std::pair<std::vector<SDFUnit>, std::vector<SDFUnit>> computeFaceVIS(face_iterator face, float angle, int rayCount);
	std::pair<std::vector<SDFUnit>, std::vector<SDFUnit>> computeFaceVIS(float angle, int rayCount);

	void generateFeatures(const char*, const char*,int);
};

#endif