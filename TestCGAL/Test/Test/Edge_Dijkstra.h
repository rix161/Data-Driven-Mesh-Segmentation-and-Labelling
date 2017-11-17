#ifndef __EDGE_DIJSTRA__
#define __EDGE_DIJSTRA__
#include "CGAL_geodesic.h"
#include "Common_Djikstra.h"
#include "GenericMesh.h"

class EdgeDijkstra : public GenericMesh {

public:
	EdgeDijkstra(Triangle_mesh mesh) {
		mMainMesh = mesh;
		mIsValid = true;
	};

	void compute();
	double getAverageGeodesicDistance(vertex_iterator point);
	//std::vector<DjiElement> getNeighbors(vertex_iterator inVertext, std::map<Kernel::Point_3, double>);
	std::vector<DjiElement> getNeighbors(vertex_iterator inVertext, double cValue);
};
#endif 
