#include "Off_Writer.h"
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>

GenericMesh OffWriter::ReadFile(const char* fileName) {
	std::ifstream inFile;
	std::vector<std::string> fields;
	std::vector<Kernel::Point_3> vData;
	std::vector<std::vector<int>> vColor;
	std::vector<std::deque<int>> fData;
	std::vector<std::vector<int>> fColor;

	GenericMesh outMesh;

	inFile.open(fileName);
	if (!inFile)
		return outMesh;
	
	std::string tempString;
	getline(inFile, tempString);	//Remove "OFF" Header
	getline(inFile, tempString);

	boost::split(fields, tempString, boost::is_any_of(" "));
	if (fields.size() != 3) {
		return outMesh;
	}

	int vertexCount = std::atoi(fields[0].c_str());
	int faceCount = std::atoi(fields[1].c_str());
	int edgeCount = std::atoi(fields[2].c_str());

	for (int i = 0; i < vertexCount; i++) {

		getline(inFile, tempString);
		boost::split(fields, tempString, boost::is_any_of(" "));
		if (fields.size() < 3) return outMesh;

		Kernel::Point_3 vPoint(std::atof(fields[0].c_str()), std::atof(fields[1].c_str()), std::atof(fields[2].c_str()));
		vData.push_back(vPoint);
		std::vector<int>tempColor;
		if(fields.size() > 3 && fields[3]!="") {
			tempColor.push_back(atoi(fields[3].c_str()));
			tempColor.push_back(atoi(fields[4].c_str()));
			tempColor.push_back(atoi(fields[5].c_str()));
			tempColor.push_back(atoi(fields[6].c_str()));
			vColor.push_back(tempColor);
		}
	}

	for (int i = 0; i < faceCount; i++) {

		getline(inFile, tempString);
		boost::split(fields, tempString, boost::is_any_of(" "));
		if (fields.size() < 3) return outMesh;

		int faceCount2 = std::atoi(fields[0].c_str());
		std::deque<int> tempq;
		for (int k = 0; k < faceCount2; k++) {
			tempq.push_back(std::atoi(fields[k + 1].c_str()));
		}

		fData.push_back(tempq);
		std::vector<int>tempColor;
		if (fields.size() > 4) {
			tempColor.push_back(atoi(fields[faceCount2 + 1].c_str()));
			tempColor.push_back(atoi(fields[faceCount2 + 2].c_str()));
			tempColor.push_back(atoi(fields[faceCount2 + 3].c_str()));
			tempColor.push_back(1);
			fColor.push_back(tempColor);
		}

	}

	outMesh.setData(vData, vColor, fData, fColor, vertexCount, faceCount, edgeCount);
	inFile.close();
	return outMesh;
}

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
			//std::cout<< "FaceData:" << pt[0] << " " << pt[1] << " " << pt[2] << " " << pt[3] << " ";
			mfile << pt[0] << " " << pt[1] << " " << pt[2]<<" "<<pt[3]<<" ";
			if (ColorfcIt != faceColor.end()) {
				std::vector<int> color = *ColorfcIt;
				mfile << color[0] << " " << color[1] << " " << color[2];
				ColorfcIt++;
			}
			mfile << std::endl;
		}
		
	}



