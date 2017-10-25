#include "Off_Writer.h"

void OffWriter::writeFile(const char* fileName,
	std::vector<Kernel::Point_3> pointList,
	std::vector<std::vector<int>> pointColor,
	std::vector<std::deque<int>> faceList,
	std::vector<std::vector<int>> faceColor,
	int numOfVertices,
	int numOfFaces,
	int numOfEdges) {
		
		std::ofstream mfile;
		
		mfile.open(fileName);
		if (!mfile)
			return;
		
		mfile << "OFF" << std::endl;
		mfile << numOfVertices << " " << numOfFaces << " " << numOfEdges << std::endl;

		std::vector<std::vector<int>>::iterator ColorptIt = pointColor.begin();

		for (std::vector<Kernel::Point_3>::iterator it = pointList.begin(); it != pointList.end(); it++) {
			Kernel::Point_3 pt = *it;
			mfile << pt.x() << " " << pt.y() << " " << pt.z();
			if (ColorptIt != pointColor.end()) {
				std::vector<int> color = *ColorptIt;
				mfile << color[0] << " " << color[1] << " " << color[2];
				ColorptIt++;
			}
			mfile << std::endl;
		}

		std::vector<std::vector<int>>::iterator ColorfcIt = faceColor.begin();
		for (std::vector<std::deque<int>>::iterator it = faceList.begin(); it != faceList.end(); it++) {
			std::deque<int> pt = *it;
			mfile << pt[0] << " " << pt[1] << " " << pt[2]<<" "<<pt[3]<<" ";
			if (ColorfcIt != faceColor.end()) {
				std::vector<int> color = *ColorfcIt;
				mfile << color[0] << " " << color[1] << " " << color[2];
				ColorfcIt++;
			}
			mfile << std::endl;
		}
		
	}



