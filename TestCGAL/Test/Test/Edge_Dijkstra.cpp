#include "Edge_Dijkstra.h"
#include "Off_Writer.h"
#include <boost/algorithm/string.hpp>


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

void EdgeDijkstra::generateFeatures(const char* fileName) {
	if (mMainMesh.is_empty()) return;

	std::ofstream curFile;

	curFile.open(fileName);
	if (!curFile)
		return;

	for (vertex_iterator vi = mMainMesh.vertices_begin(); vi != mMainMesh.vertices_end(); vi++) {
		double avgGeoDesic  = getAverageGeodesicDistance(vi);
		curFile << (int)(*vi) << "\t" << avgGeoDesic << "\n";
	}

};

void EdgeDijkstra::generateFeatures(const char* source, const char* destination) {

	std::ifstream inFile;
	std::ofstream outFile;

	inFile.open(source);
	if (!inFile)
		return;


	outFile.open(destination);
	if (!outFile)
		return;

	std::string line;

	double *agdVector = new double[mMainMesh.number_of_vertices()];
	std::memset(agdVector, 0, sizeof(mMainMesh.number_of_vertices()));
	double *agdFaceVector = new double[mMainMesh.number_of_faces()];
	std::memset(agdFaceVector, 0, sizeof(mMainMesh.number_of_faces()));
	double *agdFaceArea = new double[mMainMesh.number_of_faces()];
	std::memset(agdFaceArea, 0, sizeof(mMainMesh.number_of_faces()));


	while (std::getline(inFile, line)) {
		std::vector<std::string> substrs;
		boost::split(substrs, line, boost::is_any_of("\t"));
		int vCount = atoi(substrs[0].c_str());
		double AGDValue = atof(substrs[1].c_str())/mMainMesh.number_of_vertices();
		agdVector[vCount] = AGDValue;
	}

	double avgAgd = 0;
	double avgArea = 0;

	for (face_iterator fit = mMainMesh.faces_begin(); fit != mMainMesh.faces_end(); fit++) {
		double avgFaceAgd = 0;
		int count = 0;
		CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
		std::vector<Kernel::Point_3> vertices;
		for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*fit), mMainMesh);
			vbegin != vend; vbegin++) {
			count++;
			avgFaceAgd += agdVector[*vbegin];
			vertices.push_back(mMainMesh.point(*vbegin));
		}
		agdFaceVector[*fit] = avgFaceAgd/count;
		agdFaceArea[*fit] = sqrt(CGAL::squared_area(vertices[0], vertices[1], vertices[2]));
		avgAgd += agdFaceVector[*fit];
		avgArea += agdFaceArea[*fit];
	}

	for (face_iterator fit = mMainMesh.faces_begin(); fit != mMainMesh.faces_end(); fit++) {
		outFile << (int)(*fit) << "\t"
		<< agdFaceVector[*fit]*(agdFaceArea[*fit]/avgArea) << "\t"
		<< pow(agdFaceVector[*fit],2) * (agdFaceArea[*fit] / avgArea) << "\t"
		<< pow(agdFaceVector[*fit], 4) * (agdFaceArea[*fit] / avgArea) << "\t"
		<< pow(agdFaceVector[*fit], 8) * (agdFaceArea[*fit] / avgArea) << "\t"
		<< pow(agdFaceVector[*fit], 0.5) * (agdFaceArea[*fit] / avgArea) << "\t"
		<< pow(agdFaceVector[*fit], 0.25) * (agdFaceArea[*fit] / avgArea) << "\t"
		<< "\n";
	}
	
	delete agdVector;
	delete agdFaceVector;
	delete agdFaceArea;
}

void EdgeDijkstra::compute() {

	int binCount = 5;
	double min = 9999;
	double max = -9999;
	OffWriter writter;
	std::map<Kernel::Point_3, double> agdMap;
	int vCount = 0;
	vertexIndex.clear();
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
	
	faceIndex.clear();
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