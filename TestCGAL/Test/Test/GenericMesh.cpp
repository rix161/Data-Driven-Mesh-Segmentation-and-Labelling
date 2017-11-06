#include "GenericMesh.h"

void GenericMesh::crunchMesh() {
	
	mVertexCount = mMainMesh.number_of_vertices();
	mFaceCount = mMainMesh.number_of_faces();
	mEdgeCount = mMainMesh.number_of_edges();

	for (vertex_iterator vIter = mMainMesh.vertices_begin(); vIter != mMainMesh.vertices_end(); vIter++) {
		vertexIndex.push_back(mMainMesh.point((*vIter)));
	}

	for (face_iterator fIter = mMainMesh.faces_begin(); fIter != mMainMesh.faces_end(); fIter++) {
		CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
		std::deque<int> tempVIndex;
		for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*fIter), mMainMesh);
			vbegin != vend; vbegin++) {
			tempVIndex.push_back((int)(*vbegin));
		}
		faceIndex.push_back(tempVIndex);
	}

	mIsValid = true;
}