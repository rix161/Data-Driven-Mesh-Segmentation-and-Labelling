#ifndef OFF_WRITER_HEADER
#define OFF_WRITER_HEADER
#include "CGAL_geodesic.h"
#include "GenericMesh.h"
#include<vector>
#include <iostream>
#include <fstream>


class OffWriter{

public:
	OffWriter() {}

	void writeFile(
		const char* fileName,
		std::vector<Kernel::Point_3> pointList,
		std::vector<std::vector<int>> pointColor,
		std::vector<std::deque<int>> FaceList,
		std::vector<std::vector<int>> faceColor,
		int numOfVertices,
		int numOfFaces,
		int numOfEdges);
	
	GenericMesh ReadFile(const char*);
	
};

#endif
