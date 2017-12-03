#ifndef __EDGE_DIJSTRA__
#define __EDGE_DIJSTRA__
#include "Common_Djikstra.h"
#include "GenericMesh.h"

class EdgeDijkstra : public GenericMesh {

public:
	EdgeDijkstra(Triangle_mesh mesh) {
		mMainMesh = mesh;
		mIsValid = true;
	};

	EdgeDijkstra(const char* fileName) {
		std::ifstream in(fileName);
		in >> this->mMainMesh;
		std::cout << mMainMesh.number_of_vertices() << std::endl;
		in.close();
		mIsValid = true;
	};

	void compute();
	double getAverageGeodesicDistance(vertex_iterator point);
	//std::vector<DjiElement> getNeighbors(vertex_iterator inVertext, std::map<Kernel::Point_3, double>);
	std::vector<DjiElement> getNeighbors(vertex_iterator inVertext, double cValue);
	void generateFeatures(const char*);
	void generateFeatures(const char* filename,const char* destination);
};
#endif 
