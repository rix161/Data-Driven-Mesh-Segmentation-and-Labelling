#include <cstdlib>
#include <iostream>
#include <fstream>
#include "FeatureUnit.h"
#include "UnifiedRenderer.h"


#include "ShapeContext.h"
#define RENDERING_MODE

std::vector<FeatureUnit> getMeshFiles(const char* meshDir, const char* featureDir) {
	std::vector<FeatureUnit> units;

	if (!boost::filesystem::exists(meshDir)) { 
				return units;
	}

	if (!boost::filesystem::exists(featureDir) && !boost::filesystem::create_directory(featureDir)){
			return units;
	}

	std::vector<std::string> featureDirs = { std::string("curvature"),std::string("VIS"),std::string("SDF"),std::string("AGD"),std::string("ShapeContext") };
	for (std::string dir : featureDirs) {
		std::string fDir = featureDir + std::string("//") + dir;
		if (!boost::filesystem::exists(fDir) && !boost::filesystem::create_directory(fDir)) {
			return units;
		}
	}

	boost::filesystem::directory_iterator endIter;
	for (boost::filesystem::directory_iterator dirIter(meshDir); dirIter != endIter; dirIter++) {
		if (boost::filesystem::is_regular_file(dirIter->path()) &&
			std::strcmp(boost::filesystem::extension(dirIter->path()).c_str(), ".off") == 0) {
				boost::filesystem::path p(dirIter->path());
				units.push_back(FeatureUnit(meshDir,featureDir,p.stem().string().c_str()));
		}
	}
	return units;
}


int main(int argc, char** argv){

#ifdef RENDERING_MODE
	UnifiedRenderer uRenderer(argc, argv, 1024, 1024, "Mesh Features");
	uRenderer.setupRendererParameters();
	uRenderer.render();
	return 0;
#else

	if (argc != 3) {
		std::cout << "Usage:program.exe <Mesh Directory> <Feature Directory>";
		return 0;
	}
	const char* meshDir = argv[1];
	const char* featureDir = argv[2];

	std::vector<FeatureUnit> fUnit = getMeshFiles(meshDir, featureDir);
	for (FeatureUnit unit : fUnit) {
		ShapeContext shapeContextFile(unit.getMeshFileName().c_str(),unit.getAGDRawFileName().c_str());
		shapeContextFile.crunchMesh();
		shapeContextFile.generateFeatures(unit.getShapeContextFileName().c_str());
	}
#endif

} 