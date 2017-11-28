#include <cstdlib>
#include <iostream>
#include <fstream>
#include "CGAL_geodesic.h"
#include "Edge_Dijkstra.h"
#include "Fast_Marching_Plane.h"
#include "ShapeDiameterFunction.h"
#include "Off_Writer.h"
#include "AGDRenderer.h"
#include "SDFRenderer.h"
#include "SDFUnit.h"
#include "Curvature.h"
#include "VIS.h"
#include "FeatureUnit.h"
#include <boost/filesystem.hpp>
#include "UnifiedRenderer.h"


std::vector<FeatureUnit> getMeshFiles(const char* meshDir, const char* featureDir) {
	std::vector<FeatureUnit> units;

	if (!boost::filesystem::exists(meshDir)) { 
				return units;
	}

	if (!boost::filesystem::exists(featureDir) && !boost::filesystem::create_directory(featureDir)){
			return units;
	}

	std::vector<std::string> featureDirs = { std::string("curvature"),std::string("VIS"),std::string("SDF"),std::string("AGD") };
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


int main(int argc, char** argv)
{
	
	if (argc != 3) {
		std::cout << "Usage:program.exe <Mesh Directory> <Feature Directory>";
		return 0;
	}
	
	UnifiedRenderer uRenderer(argc, argv, 1024, 1024, "Mesh Features");
	uRenderer.setupRendererParameters();
	uRenderer.render();
	return 0;
} 