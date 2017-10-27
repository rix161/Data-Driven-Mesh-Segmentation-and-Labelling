#include <cstdlib>
#include <iostream>
#include <fstream>
#include "CGAL_geodesic.h"
#include "Edge_Dijkstra.h"
#include "Fast_Marching_Plane.h"
#include "Off_Writer.h"
#include "AGDRenderer.h"

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
	fmPlane.compute();*/

	OffWriter writter;
	GenericMesh mesh = writter.ReadFile("F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\output\\output_king.off");
	
	AGDRenderer myRenderer(argc, argv, 1024, 1024, "AGDRenderer");
	myRenderer.setupRendererParameters(
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDVertexShader.vs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDFragmentShader.fs",
		mesh);
	
	glutMainLoop();
	return 0;
} 