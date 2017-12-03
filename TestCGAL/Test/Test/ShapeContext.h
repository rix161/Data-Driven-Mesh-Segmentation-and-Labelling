#ifndef __SHAPE_CONTEXT__
#define __SHAPE_CONTEXT__
#include "GenericMesh.h"
#include "Off_Writer.h"
#include <boost/algorithm/string.hpp>
#include "ShapeContextUnit.h"

class ShapeContext : public GenericMesh {
private:
	double *agdFaceVector;
	double mMaxGeoVal;
	double mMinGeoVal;
	std::vector<ShapeContextUnit> mSCUnits;
	
public:
	ShapeContextUnit compute();
	ShapeContextUnit compute(face_iterator fIter);
	void generateFeatures(const char*);

	//Taken from https://www.mathworks.com/matlabcentral/answers/328240-calculate-the-3d-angle-between-two-vectors
	double computeAngle(Kernel::Vector_3 normal, Kernel::Vector_3 angleVector) {
		Kernel::Vector_3 crossVec = CGAL::cross_product(normal, angleVector);
		crossVec = crossVec / CGAL::sqrt(crossVec[0] * crossVec[0] + crossVec[1] * crossVec[1] + crossVec[2] * crossVec[2]);
		double dotVec = normal*angleVector;
		return atan2(CGAL::sqrt(crossVec.squared_length()), dotVec);
	}

	std::vector<Kernel::Point_3> getFaceVertices(face_iterator fIter) {
		std::vector<Kernel::Point_3> faceVertices;
		CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
		for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*fIter), mMainMesh);
			vbegin != vend; vbegin++) {
			faceVertices.push_back(mMainMesh.point((*vbegin)));
		}
		return faceVertices;
	}

	ShapeContext(const char* meshFileName,const char* agdFileName) {

		std::ifstream in(meshFileName);
		in >> this->mMainMesh;
		std::cout << mMainMesh.number_of_vertices() << std::endl;
		in.close();
		mIsValid = true;

		std::ifstream agdFile;
		agdFile.open(agdFileName);
		if (!agdFile)
			return;

		mMaxGeoVal = -9999999;
		mMinGeoVal =  9999999;
		
		double *agdVector = new double[mMainMesh.number_of_vertices()];
		std::memset(agdVector, 0, sizeof(mMainMesh.number_of_vertices()));
		agdFaceVector = new double[mMainMesh.number_of_faces()];
		std::memset(agdFaceVector, 0, sizeof(mMainMesh.number_of_faces()));

		std::string line;
		while (std::getline(agdFile, line)) {
			std::vector<std::string> substrs;
			boost::split(substrs, line, boost::is_any_of("\t"));
			int vCount = atoi(substrs[0].c_str());
			double AGDValue = atof(substrs[1].c_str()) / mMainMesh.number_of_vertices();
			agdVector[vCount] = AGDValue;
		}

		double avgAgd = 0;
		double avgArea = 0;

		for (face_iterator fit = mMainMesh.faces_begin(); fit != mMainMesh.faces_end(); fit++) {
			double avgFaceAgd = 0;
			int count = 0;
			CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
			std::vector<Kernel::Point_3> vertices;
			for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*fit), mMainMesh);
				vbegin != vend; vbegin++) {
				count++;
				avgFaceAgd += agdVector[*vbegin];
				vertices.push_back(mMainMesh.point(*vbegin));
			}
			agdFaceVector[*fit] = avgFaceAgd / count;

			if (agdFaceVector[*fit] > mMaxGeoVal) mMaxGeoVal = agdFaceVector[*fit];
			if (agdFaceVector[*fit] < mMinGeoVal) mMinGeoVal = agdFaceVector[*fit];

			avgAgd += agdFaceVector[*fit];
		}
		agdFile.close();
		delete agdVector;
	}

	~ShapeContext() {
		if (agdFaceVector)
			delete agdFaceVector;
	}

};

#endif