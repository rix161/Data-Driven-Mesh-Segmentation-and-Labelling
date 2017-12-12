#ifndef __FAST_MARCHING_PLANE__
#define __FAST_MARCHING_PLANE__
#include "Common_Djikstra.h"
#include "GenericMesh.h"
#include "Edge_Dijkstra.h"
#include "updatePriorityQueue.h"

class FastMarchingPlanes: public EdgeDijkstra {
private:
	double **lookUp;

public:
	
	FastMarchingPlanes(Triangle_mesh mesh) :EdgeDijkstra(mesh) {

		/*lookUp = new double*[mMainMesh.number_of_vertices()];
		
		for (int i = 0; i < mMainMesh.number_of_vertices(); i++) {
			lookUp[i] = new double[mMainMesh.number_of_vertices()];
			memset(lookUp[i], -1, mMainMesh.number_of_vertices());
		}*/


	}

	FastMarchingPlanes(const char* fileName) :EdgeDijkstra(fileName) {}

	~FastMarchingPlanes() {

		/*for (int i = 0; i < mMainMesh.number_of_vertices(); i++) {
			delete lookUp[i];
		}
		delete lookUp;*/
	}

	void compute();
	double getAverageGeodesicDistance(vertex_iterator point);
	void FastMarchingPlanes::computeUpdate(DjiElement trial, 
		std::map<Kernel::Point_3, double> &vertexMap, 
		UpdatablePriorityQueue < DjiElement, std::vector<DjiElement>, compare> &minQueue, 
		std::set<Kernel::Point_3> fixedSet, 
		std::set<Kernel::Point_3> &closedSet, 
		std::set<Kernel::Point_3> &upProcessedSet);
	void generateFeatures(const char*);
};
#endif