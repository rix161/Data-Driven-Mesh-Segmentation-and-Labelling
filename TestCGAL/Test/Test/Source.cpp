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
	const char* meshDir = argv[1];
	const char* featureDir = argv[2];
	
	const char* fileName = "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\PaperData\\ColoredMesh\\real\\201.off";

	/*std::vector<FeatureUnit> fUnit = getMeshFiles(meshDir, featureDir);
	for (FeatureUnit unit : fUnit) {
		/*std::cout << "Generating AGD Features for:" << unit.getMeshFileName() << " in:" << unit.getAGDFileName() << std::endl;
		Curvature c(unit.getMeshFileName().c_str());
		c.crunchMesh();
		c.generateFeatures(unit.getCurvatureFileName().c_str());
		std::cout << "Generating AGD Features for:" << unit.getMeshFileName() << " in:" << unit.getAGDFileName() << std::endl;
		CGALGeodesic cgalGeo(unit.getMeshFileName().c_str());
		cgalGeo.crunchMesh();
		cgalGeo.generateFeatures(unit.getAGDFileName().c_str());
	}*/


	/*std::vector<FeatureUnit> fUnit = getMeshFiles(meshDir, featureDir);
	for (FeatureUnit unit : fUnit) {
		std:: cout<< "Generating Curvature Features for:" << unit.getMeshFileName() << " in:" << unit.getCurvatureFileName() << std::endl;
		Curvature c(unit.getMeshFileName().c_str());
		c.crunchMesh();
		c.generateFeatures(unit.getCurvatureFileName().c_str());
	}*/

	/*for (FeatureUnit unit : fUnit) {
		std::cout << "Generating SDF and VIS Features for:" << unit.getMeshFileName() << " in:" << unit.getSDFFileName() << std::endl;
		VIS c(unit.getMeshFileName().c_str());
		c.crunchMesh();
		c.generateFeatures(unit.getSDFFileName().c_str(), unit.getVISFileName().c_str(),20);
	}*/



	//const char* fileName = "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\PaperData\\labeledDb\\LabeledDB_new\\Bearing\\359.off";

	/*Curvature cMesh(fileName);
	cMesh.crunchMesh();
	cMesh.generateFeatures("F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\PaperData\\labeledDb_features\\Bearing\\mushroom_curve.txt");*/

	OffWriter writter;
	GenericMesh cMesh = writter.ReadFile(fileName);

	Triangle_mesh tmesh;
	AGDRenderer myRenderer(argc, argv, 1024, 1024, "AGDRenderer");
	myRenderer.setupRendererParameters(
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDVertexShader.vs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDFragmentShader.fs",
		cMesh);

	/*Triangle_mesh tmesh2;
	std::ifstream in2(fileName);
	in2 >> tmesh2;
	in2.close();

	OffWriter writter;
	GenericMesh mesh = writter.ReadFile(fileName);
	int randnum = rand() % (mesh.getFaceCount() + 1);
	randnum = rand() % (mesh.getFaceCount() + 1);
	randnum = rand() % (mesh.getFaceCount() + 1);
	randnum = rand() % (mesh.getFaceCount() + 1);
	randnum = rand() % (mesh.getFaceCount() + 1);
	randnum = rand() % (mesh.getFaceCount() + 1);
	face_iterator iter = tmesh2.faces_begin();
	std::advance(iter, randnum);

	ShapeDiameterFunction sdf(fileName);
	sdf.crunchMesh();
	std::vector<SDFUnit> rays = sdf.compute(iter, 60, 30)

	VIS vis(fileName);
	vis.crunchMesh();
	std::pair<std::vector<SDFUnit>, std::vector<SDFUnit>> rays2 = vis.computeFaceVIS(iter,30,30);

	SDFRenderer myRenderer(argc, argv, 1024, 1024, "SDFRenderer");

	myRenderer.setupRendererParameters(
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFVertexShader.vs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFFragmentShader.fs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFRayVertextShader.vs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFRayFragmentShader.fs",
		vis,
		rays2.second);*/


	glutMainLoop();

	/*VIS vis(fileName);
	vis.crunchMesh();
	vis.generateFeatures("F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\PaperData\\labeledDb_features\\Bearing\\354_sdf.txt", "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\PaperData\\labeledDb_features\\Bearing\\354_vis.txt",);
	
	
	glutMainLoop();*/
	return 0;
} 