#ifndef  __GENERIC_MESH__
#define __GENERIC_MESH__
#include "CGAL_geodesic.h"

class GenericMesh {

protected:
	Triangle_mesh mMainMesh;
	std::vector<Kernel::Point_3>vertexIndex;
	std::vector<std::vector<int>>vectexColor;
	std::vector<std::deque<int>>faceIndex;
	std::vector<std::vector<int>>faceColor;
	int mVertexCount, mFaceCount, mEdgeCount;
	bool mIsValid;
public:
	void setData(std::vector<Kernel::Point_3> vData,
		std::vector<std::vector<int>> vColor,
		std::vector<std::deque<int>> fData,
		std::vector<std::vector<int>> fColor,
		int vCount,
		int fCount,
		int eCount) {

		this->vertexIndex = vData;
		this->vectexColor = vColor;
		this->faceIndex = fData;
		this->faceColor = fColor;
		mVertexCount = vCount;
		mFaceCount = fCount;
		mEdgeCount = eCount;
	}

	std::vector<Kernel::Point_3> getVertexIndex() { return vertexIndex; };
	std::vector<std::vector<int>> getVertexColor() { return vectexColor; }
	std::vector<std::deque<int>> getFaceIndex() { return faceIndex; }
	std::vector<std::vector<int>> getFaceColor() { return faceColor; }
	int getVertexCount() { return mMainMesh.number_of_vertices();}
	int getFaceCount() { return mMainMesh.number_of_faces(); }
	int getEdgeCount() { return mMainMesh.number_of_edges(); }
	bool isValid() {return mIsValid;}
};
#endif // ! __GENERIC_MESH__
