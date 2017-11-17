
#include "CGAL_geodesic.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

#define DEBUGMODE

void CGALGeodesic::generateFeatures(const char* fileName) {

	if (mMainMesh.is_empty()) return;


	std::ofstream curFile;

	curFile.open(fileName);
	if (!curFile)
		return;

	Surface_mesh_shortest_path shortest_paths(mMainMesh);
	double **dpMatrix = new double*[mMainMesh.number_of_vertices()];
	for (int i = 0; i < mMainMesh.number_of_vertices(); i++) {
		dpMatrix[i] = new double[mMainMesh.number_of_vertices()];
		std::memset(dpMatrix[i], 0, sizeof(mMainMesh.number_of_vertices()));
		for (int j = 0; j < mMainMesh.number_of_vertices(); j++) {
			dpMatrix[i][j] = -1;
		}
	}

	vertex_iterator vit, vit_end;
	int count = -1;
	auto start =std::chrono::system_clock::now();
	for (boost::tie(vit, vit_end) = vertices(mMainMesh); vit != vit_end; ++vit) {
		count++;
		shortest_paths.add_source_point(*vit);
		double avgGeodesicDistance = 0.0f;

		vertex_iterator lvit, lvit_end;
		for (boost::tie(lvit, lvit_end) = vertices(mMainMesh); lvit != lvit_end; ++lvit) {
			if ((int)(*vit) == (int)(*lvit)) continue;
			if (dpMatrix[(int)(*vit)][(int)(*lvit)] != -1){
				//std::cout << "Something count" << count << std::endl;
				avgGeodesicDistance += dpMatrix[(int)(*vit)][(int)(*lvit)];
				continue;
			}
				
			Surface_mesh_shortest_path::Shortest_path_result result = shortest_paths.shortest_distance_to_source_points(*lvit);
#ifdef DEBUGMODE_TEMP
			//std::cout << "GeoDesic distance:" << result.first << " Point1:" << mMainMesh.point(*lvit) << " Point2:" << mMainMesh.point(*vit) << std::endl;
#endif
			avgGeodesicDistance += result.first;
			dpMatrix[(int)(*vit)][(int)(*lvit)] = result.first;
			dpMatrix[(int)(*lvit)][(int)(*vit)] = result.first;
		}

		Kernel::Point_3 point = mMainMesh.point(*vit);
#ifdef DEBUGMODE
		if (count % 100 == 0) {
			std::chrono::seconds temp= std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()) - std::chrono::duration_cast<std::chrono::seconds>(start.time_since_epoch());
			std::cout << "count" << count << "AVG GeoDesic distance:" << avgGeodesicDistance << " Point:" << mMainMesh.point(*vit)<<" duration"<< temp.count()<< std::endl;
			start = std::chrono::system_clock::now();
		}
#endif
		curFile << (int)(*vit) << "\t" << avgGeodesicDistance << "\n";
		shortest_paths.remove_all_source_points();
	}
}


double CGALGeodesic::getAverageGeodesicDistance() {

	if (mMainMesh.is_empty()) return 0.0;
	vertex_iterator sourceVertex = mMainMesh.vertices().begin();
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


