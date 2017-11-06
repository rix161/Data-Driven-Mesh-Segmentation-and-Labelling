#include "Edge_Dijkstra.h"
#include "Off_Writer.h"



void setupColorBins(std::vector<std::vector<int>> &colorBins) {

	for (int i = 0; i < colorBins.size(); i++) {
		std::vector<int> color(4);
		color[0] = rand() % 255;
		color[1] = rand() % 255;
		color[2] = rand() % 255;
		color[3] = 0;
		colorBins[i] = color;
	}
}

void EdgeDijkstra::compute() {

	int binCount = 5;
	double min = 9999;
	double max = -9999;
	OffWriter writter;
	std::map<Kernel::Point_3, double> agdMap;
	int vCount = 0;
	for (vertex_iterator vi = mMainMesh.vertices_begin(); vi != mMainMesh.vertices_end(); vi++) {
		if (vCount % 100 == 0)
			std::cout << "Vcount: " << vCount << std::endl;
		vCount++;
		double avgDistanceDjik = getAverageGeodesicDistance(vi);

		Kernel::Point_3 pt = mMainMesh.point(*vi);
		agdMap[pt] = avgDistanceDjik;
		vertexIndex.push_back(pt);
		if (avgDistanceDjik > max) max = avgDistanceDjik;
		if (avgDistanceDjik < min) min = avgDistanceDjik;
	}

	double range = (max - min) / binCount;
	std::vector<std::vector<int>> colorBin(binCount);
	setupColorBins(colorBin);

	for (face_iterator fit = mMainMesh.faces().begin(); fit != mMainMesh.faces().end(); fit++) {
		CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
		double avgGd = 0.0;
		int count = 0;
		std::deque<int> vertexIndex;
		for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*fit), mMainMesh);
			vbegin != vend; vbegin++) {
			vertexIndex.push_back((int)(*vbegin));
			avgGd += agdMap[mMainMesh.point(*vbegin)];
			count++;
		}
		vertexIndex.push_front(count);
		faceIndex.push_back(vertexIndex);

		avgGd /= count;
		int binNum = floor((avgGd - min) / range);
		std::vector<int> colorVec = colorBin[binNum];
		faceColor.push_back(colorVec);
	}

	writter.writeFile("F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\output\\output.off",
		vertexIndex,
		vectexColor,
		faceIndex,
		faceColor,
		mMainMesh.number_of_vertices(),
		mMainMesh.number_of_faces(),
		mMainMesh.number_of_edges());
}

std::vector<DjiElement> EdgeDijkstra::getNeighbors(vertex_iterator inVertex,double cValue) {
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

		if (visitedSet.count(initElement.getPoint()) != 0 ) continue;

		visitedSet.insert(initElement.getPoint());

		//std::vector<DjiElement> neighbours = getNeighbors(initElement.getVertex(), vertexMap[mMainMesh.point(*(initElement.getVertex()))]);
		std::vector<DjiElement> neighbours;
		CGAL::Vertex_around_target_circulator<Triangle_mesh> vbegin(mMainMesh.halfedge(*initElement.getVertex()), mMainMesh), done(vbegin);
		do {
			vertex_iterator iter = vertices(mMainMesh).begin();
			std::advance(iter, *vbegin);
			DjiElement neighbor(iter, mMainMesh.point(*iter), vertexMap[mMainMesh.point(*(initElement.getVertex()))]);
			neighbours.push_back(neighbor);
			vbegin++;
		} while (vbegin != done);
		
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
		neighbours.clear();
	}

	double sum = 0.0;
	int count = 0;
	for (std::map<Kernel::Point_3, double>::iterator itr = vertexMap.begin(); itr != vertexMap.end(); itr++) {
		count++;
		sum += itr->second;
	}

	return sum / count;
}