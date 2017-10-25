#ifndef __EDGE_DIJSTRA__
#define __EDGE_DIJSTRA__
#include "CGAL_geodesic.h"
#include "Common_Djikstra.h"
#include <vector>

class EdgeDijkstra{
protected:
	Triangle_mesh mMainMesh;

public:
	EdgeDijkstra(Triangle_mesh mesh) {
		mMainMesh = mesh;
	};

	void compute();
	double getAverageGeodesicDistance(vertex_iterator point);
	std::vector<DjiElement> getNeighbors(vertex_iterator inVertext, std::map<Kernel::Point_3, double>);
};
#endif 
