#ifndef __CGAL__GEODESIC__
#define __CGAL__GEODESIC__

#include "GenericMesh.h"

class CGALGeodesic : public GenericMesh {

public:
	
	CGALGeodesic(const char* fileName) {
		std::ifstream in(fileName);
		in >> this->mMainMesh;
		std::cout << mMainMesh.number_of_vertices() << std::endl;
		in.close();
	};
	
	void generateFeatures(const char* fileName);
	double getAverageGeodesicDistance();
};
#endif 



