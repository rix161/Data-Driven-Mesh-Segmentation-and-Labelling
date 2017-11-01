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

int main(int argc, char** argv)
{
	/*Triangle_mesh tmesh;

	std::ifstream in("F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\output\\cube.off");
	in >> tmesh;
	in.close();
	
	CGALGeodesic cgalGeo(tmesh);


	double avgDistace = cgalGeo.getAverageGeodesicDistance(tmesh.vertices().begin());
	std::cout << "AverageDistance: " << avgDistace<<std::endl;

	EdgeDijkstra edgeGeo(tmesh);
	double avgDistanceDjik = edgeGeo.getAverageGeodesicDistance(tmesh.vertices().begin());
	std::cout << "NaiveDjikstra: " << avgDistanceDjik << std::endl;

	FastMarchingPlanes fmPlane(tmesh);
	double avgDistancefMP = fmPlane.getAverageGeodesicDistance(tmesh.vertices().begin());
	std::cout << "FastMarching: " << avgDistancefMP << std::endl;
	fmPlane.compute();

	OffWriter writter;
	GenericMesh mesh = writter.ReadFile("F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\output\\output_king_unit.off");
	
	AGDRenderer myRenderer(argc, argv, 1024, 1024, "AGDRenderer");
	myRenderer.setupRendererParameters(
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDVertexShader.vs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDFragmentShader.fs",
		mesh);*/

	Triangle_mesh tmesh2;
	std::ifstream in2("F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\output\\output_king_unit.off");
	in2 >> tmesh2;
	in2.close();
	
	OffWriter writter;
	GenericMesh mesh = writter.ReadFile("F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\output\\output_king_unit.off");


	ShapeDiameterFunction sdf;
	int randnum = rand() % ( mesh.getFaceCount()+ 1);
	randnum = rand() % (mesh.getFaceCount() + 1);
	randnum = rand() % (mesh.getFaceCount() + 1);
	face_iterator iter = tmesh2.faces_begin();
	std::advance(iter, randnum);
	std::vector<SDFUnit> rays = sdf.compute(tmesh2, iter, 60, 30);

	SDFRenderer myRenderer(argc, argv, 1024, 1024, "SDFRenderer");

	myRenderer.setupRendererParameters(
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFVertexShader.vs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFFragmentShader.fs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFRayVertextShader.vs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFRayFragmentShader.fs",
		mesh,
		rays);


	glutMainLoop();
	return 0;
} 