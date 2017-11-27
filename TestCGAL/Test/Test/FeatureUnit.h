#ifndef __FEATURE_UNIT__
#include <string>
#include <boost/filesystem.hpp>

class FeatureUnit {
private:
	std::string fileName;
	std::string fileDirectory;
	std::string meshDirectory;
public:
	FeatureUnit(const char* meshDir,const char* featureDir, const char* fileName) {
		this->fileName = std::string(fileName);
		this->fileDirectory = std::string(featureDir);
		this->meshDirectory = std::string(meshDir);
	}

	std::string getMeshFileName() {
		std::string retString;
		retString = this->meshDirectory + "\\" + fileName + ".off";
		const char* temp = retString.c_str();
		return retString;
	}

	std::string getCurvatureFileName() {
		std::string retString;
		retString = this->fileDirectory + std::string("\\") +"curvature"+ std::string("\\")+this->fileName+".txt";
		return retString;
	}

	std::string getVISFileName() {
		std::string retString;
		retString = this->fileDirectory + std::string("\\") + "VIS" + std::string("\\") + this->fileName + ".txt";
		return retString;
	}

	std::string getSDFFileName() {
		std::string retString;
		retString = this->fileDirectory + std::string("\\") + "SDF" + std::string("\\") + this->fileName + ".txt";
		return retString;
	}

	std::string getAGDFileName() {
		std::string retString;
		retString = this->fileDirectory + std::string("\\") + "AGD" + std::string("\\") + this->fileName + ".txt";
		return retString;
	}

	std::string getAGDRawFileName() {
		std::string retString;
		retString = this->fileDirectory + std::string("\\") + "AGD_RAW" + std::string("\\") + this->fileName + ".txt";
		return retString;
	}
};

#endif