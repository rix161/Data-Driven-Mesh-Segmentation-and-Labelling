#include <cstdlib>
#include <iostream>
#include <fstream>

#include <chrono>
#include <ctime>


#include "FeatureUnit.h"
#include "UnifiedRenderer.h"

#include "ShapeContext.h"
#include "Fast_Marching_Plane.h"
#include "Curvature.h"
#include "ShapeDiameterFunction.h"
#include "VIS.h"
#include "Edge_dijkstra.h"

#define RENDERING_MODE

std::vector<FeatureUnit> getMeshFiles(const char* meshDir, const char* featureDir) {
	std::vector<FeatureUnit> units;

	if (!boost::filesystem::exists(meshDir)) { 
				return units;
	}

	if (!boost::filesystem::exists(featureDir) && !boost::filesystem::create_directory(featureDir)){
			return units;
	}

	std::vector<std::string> featureDirs = { std::string("curvature"),std::string("VIS"),std::string("SDF"),std::string("AGD"),std::string("AGD_RAW"),std::string("ShapeContext") };
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
		auto start = std::chrono::system_clock::now();
		FastMarchingPlanes FMP(unit.getMeshFileName().c_str());
		FMP.crunchMesh();
		FMP.generateFeatures(unit.getAGDRawFileName().c_str());
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::cout << "elapsed time AGD: " << elapsed_seconds.count() << "s\n";

		start = std::chrono::system_clock::now();
		ShapeContext shapeContextFile(unit.getMeshFileName().c_str(),unit.getAGDRawFileName().c_str());
		shapeContextFile.crunchMesh();
		shapeContextFile.generateFeatures(unit.getShapeContextFileName().c_str());
		end = std::chrono::system_clock::now();
		elapsed_seconds = end - start;
		std::cout << "elapsed time ShapeContext: " << elapsed_seconds.count() << "s\n";

		start = std::chrono::system_clock::now();
		VIS vis(unit.getMeshFileName().c_str());
		vis.crunchMesh();
		vis.generateFeatures(unit.getShapeContextFileName().c_str(), unit.getVISFileName().c_str(),30);
		end = std::chrono::system_clock::now();
		elapsed_seconds = end - start;
		std::cout << "elapsed time VIS and SFD: " << elapsed_seconds.count() << "s\n";

		start = std::chrono::system_clock::now();
		Curvature curve(unit.getMeshFileName().c_str());
		curve.crunchMesh();
		curve.generateFeatures(unit.getCurvatureFileName().c_str());
		end = std::chrono::system_clock::now();
		elapsed_seconds = end - start;
		std::cout << "elapsed time Curvature: " << elapsed_seconds.count() << "s\n";

	}
#endif
	getchar();

} 