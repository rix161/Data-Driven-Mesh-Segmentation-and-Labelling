#include "Edge_Dijkstra.h"

void EdgeDijkstra::compute() {


}

std::vector<DjiElement> EdgeDijkstra::getNeighbors(vertex_iterator inVertex,std::map<Kernel::Point_3, double> vertexMap) {
	double cValue = vertexMap[mMainMesh.point(*inVertex)];
	std::vector<DjiElement> returnVector;
	CGAL::Vertex_around_target_circulator<Triangle_mesh> vbegin(mMainMesh.halfedge(*inVertex), mMainMesh), done(vbegin);
	
	do {
		vertex_iterator iter = vertices(mMainMesh).begin();
		std::advance(iter, *vbegin);
		DjiElement neighbor(iter,mMainMesh.point(*iter),cValue);
		returnVector.push_back(neighbor);
		vbegin++;
	} while (vbegin != done);

	return returnVector;
}

double EdgeDijkstra::getAverageGeodesicDistance(vertex_iterator sourcePoint) {

	if (mMainMesh.is_empty()) return 0.0;
	
	DjiElement source(sourcePoint,mMainMesh.point(*sourcePoint),0.0);
	std::priority_queue < DjiElement, std::vector<DjiElement>, compare> minQueue;
	std::set<Kernel::Point_3> visitedSet;
	std::map<Kernel::Point_3,double> vertexMap;
	vertex_iterator lvit, lvit_end;
	for (boost::tie(lvit, lvit_end) = vertices(mMainMesh); lvit != lvit_end; ++lvit) {
		vertexMap[mMainMesh.point(*lvit)] = 999999.9;
 	}

	vertexMap[mMainMesh.point(*source.getVertex())] = 0.0;
	minQueue.push(source);
	
	while (!minQueue.empty()) {
		DjiElement initElement = minQueue.top();
		minQueue.pop();

		if (visitedSet.find(initElement.getPoint()) != visitedSet.end()) continue;

		visitedSet.insert(initElement.getPoint());

		std::vector<DjiElement> neighbours = getNeighbors(initElement.getVertex(),vertexMap);
		Kernel::Point_3 pPoint = mMainMesh.point(*initElement.getVertex());

		for (size_t i = 0; i < neighbours.size(); i++) {

			Kernel::Point_3 cPoint = mMainMesh.point(*neighbours.at(i).getVertex());
			double edgeDistance = CGAL::sqrt(CGAL::squared_distance(pPoint, cPoint));
			if (vertexMap[cPoint] > vertexMap[pPoint] + edgeDistance) {
				vertexMap[cPoint] = vertexMap[pPoint] + edgeDistance;
				neighbours.at(i).updateDistance(vertexMap[cPoint]);
			}

			if(visitedSet.count(neighbours.at(i).getPoint()) == 0)
				minQueue.push(neighbours.at(i));
		}
	}

	double sum = 0.0;
	int count = 0;
	for (std::map<Kernel::Point_3, double>::iterator itr = vertexMap.begin(); itr != vertexMap.end(); itr++) {
		count++;
		sum += itr->second;
	}

	return sum / count;
}