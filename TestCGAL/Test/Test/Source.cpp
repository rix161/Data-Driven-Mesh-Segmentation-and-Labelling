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

#include <ctime>
#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
	Triangle_mesh tmesh;
	std::chrono::system_clock::time_point start;
	const char* fileName = "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\TestModels\\mushroom.off";
	
	double seconds;
	//const char* fileName = "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\PaperData\\labeledDb\\LabeledDB_new\\Bearing\\354.off";
	Curvature cMesh(fileName);
	cMesh.crunchMesh();
	cMesh.computeMeshCurvature();
	
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
	std::vector<SDFUnit> rays = sdf.compute(iter, 60, 30);

	VIS vis(fileName);
	vis.crunchMesh();
	std::pair<std::vector<SDFUnit>, std::vector<SDFUnit>> rays2 = vis.computeFaceVIS(iter,30,2);

	SDFRenderer myRenderer(argc, argv, 1024, 1024, "SDFRenderer");

	myRenderer.setupRendererParameters(
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFVertexShader.vs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFFragmentShader.fs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFRayVertextShader.vs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFRayFragmentShader.fs",
		vis,
		rays2.second);


	glutMainLoop();

	VIS vis(fileName);
	vis.crunchMesh();
	vis.generateFeatures("F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\PaperData\\labeledDb_features\\Bearing\\354_sdf.txt", "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\PaperData\\labeledDb_features\\Bearing\\354_vis.txt",);
	*/
	
	glutMainLoop();
	return 0;
} 