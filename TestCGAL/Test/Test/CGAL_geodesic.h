#ifndef __CGAL__GEODESIC__

#define __CGAL__GEODESIC__

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Random.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Surface_mesh_shortest_path.h>
#include <boost/lexical_cast.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3> Triangle_mesh;
typedef CGAL::Surface_mesh_shortest_path_traits<Kernel, Triangle_mesh> Traits;
typedef CGAL::Surface_mesh_shortest_path<Traits> Surface_mesh_shortest_path;
typedef boost::graph_traits<Triangle_mesh> Graph_traits;
typedef Graph_traits::vertex_iterator vertex_iterator;
typedef Graph_traits::face_iterator face_iterator;



class CGALGeodesic {
private:
	Triangle_mesh mMainMesh;
public:
	
	CGALGeodesic(Triangle_mesh mesh) {
		mMainMesh = mesh;
	};
	
	void compute();
	double getAverageGeodesicDistance(vertex_iterator point);
};
#endif 



