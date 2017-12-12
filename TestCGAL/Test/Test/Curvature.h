#ifndef __CURVATURE_FEATURE__
#define __CURVATURE_FEATURE__
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Random.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Surface_mesh_shortest_path.h>
#include <boost/lexical_cast.hpp>
#include <string>
#include "GenericMesh.h"
#include <iostream>
#include <fstream>
#include "CurvatureUnit.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3> Triangle_mesh;
typedef CGAL::Surface_mesh_shortest_path_traits<Kernel, Triangle_mesh> Traits;
typedef CGAL::Surface_mesh_shortest_path<Traits> Surface_mesh_shortest_path;
typedef boost::graph_traits<Triangle_mesh> Graph_traits;
typedef Graph_traits::vertex_iterator vertex_iterator;
typedef Graph_traits::face_iterator face_iterator;

class Curvature:public GenericMesh {

private:
	std::vector<double> areas;
	std::vector<Kernel::Vector_3> coord;
	std::vector<Kernel::Vector_3> normals;
	std::vector<double> kMean;
	std::vector<double> kGauss;
	std::map<Kernel::Point_3, CGAL::SM_Vertex_index> vertexMap;
	std::vector<CurvatureUnit> results;

	void computeBaryCenteric(Kernel::Point_3, Kernel::Vector_3, std::vector<Kernel::Point_3>, double &, double &, double &);
	void buildNRings(face_iterator, std::set<Kernel::Point_3> &, int n);
	void buildNRings(face_iterator, std::set<Triangle_mesh::Face_index> &, int n);
	std::vector<CurvatureUnit> mCurves;
public:
	Curvature(const char* fileName);
	CurvatureUnit computeFaceCurvature(face_iterator);
	void computeMeshCurvature();
	std::vector<CurvatureUnit> computeMeshCurvature2();
	void generateFeatures(const char* fileName);
	std::vector<float> computeFaceAngles(face_iterator,std::vector<Kernel::Point_3> faceVertices);
	void computeVoronialAcute(std::vector<Kernel::Point_3> faceVertices, std::vector<float> faceAngles);
	void computeVoronialObtuse(std::vector<Kernel::Point_3> faceVertices, std::vector<float> faceAngles);
	void computeGaussianAndMeanCurvature(face_iterator);
	void initVectors();
};
#endif // ! __CURVATURE_FEATURE__
