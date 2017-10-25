
#include "CGAL_geodesic.h"

#define DEBUGMODE
void CGALGeodesic::compute() {

	if (mMainMesh.is_empty()) return;

	Surface_mesh_shortest_path shortest_paths(mMainMesh);

	vertex_iterator vit, vit_end;
	for (boost::tie(vit, vit_end) = vertices(mMainMesh); vit != vit_end; ++vit) {
		shortest_paths.add_source_point(*vit);
		double avgGeodesicDistance = 0.0f;

		vertex_iterator lvit, lvit_end;
		for (boost::tie(lvit, lvit_end) = vertices(mMainMesh); lvit != lvit_end; ++lvit) {
			Surface_mesh_shortest_path::Shortest_path_result result = shortest_paths.shortest_distance_to_source_points(*lvit);
#ifdef DEBUGMODE_TEMP
			//std::cout << "GeoDesic distance:" << result.first << " Point1:" << mMainMesh.point(*lvit) << " Point2:" << mMainMesh.point(*vit) << std::endl;
#endif
			avgGeodesicDistance += result.first;
		}

		Kernel::Point_3 point = mMainMesh.point(*vit);
#ifdef DEBUGMODE
		std::cout << "AVG GeoDesic distance:" << avgGeodesicDistance << " Point:" << mMainMesh.point(*vit) << std::endl;
#endif
		shortest_paths.remove_all_source_points();
	}
}


double CGALGeodesic::getAverageGeodesicDistance(vertex_iterator sourceVertex) {

	if (mMainMesh.is_empty()) return 0.0;

	Surface_mesh_shortest_path shortest_paths(mMainMesh);
	Kernel::Point_3 sourcePoint = mMainMesh.point(*sourceVertex);
	shortest_paths.add_source_point(*sourceVertex);

		double avgGeodesicDistance = 0.0f;

		vertex_iterator lvit, lvit_end;
		for (boost::tie(lvit, lvit_end) = vertices(mMainMesh); lvit != lvit_end; ++lvit) {
			Surface_mesh_shortest_path::Shortest_path_result result = shortest_paths.shortest_distance_to_source_points(*lvit);
#ifdef DEBUGMODE_DISABLE
			std::cout << "GeoDesic distance:" << result.first << " Point1:" << mMainMesh.point(*lvit) << " Point2:" << mMainMesh.point(*sourceVertex) << std::endl;
#endif
			avgGeodesicDistance += result.first;
		}

#ifdef DEBUGMODE
		std::cout << "AVG GeoDesic distance:" << avgGeodesicDistance << " Point:" << sourcePoint << std::endl;
#endif

		return avgGeodesicDistance/mMainMesh.number_of_vertices();
}


