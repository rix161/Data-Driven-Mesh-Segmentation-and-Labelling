#include "ShapeContext.h"

ShapeContextUnit ShapeContext::compute() {

	int randnum = rand() % (mMainMesh.number_of_faces() + 1);
	randnum = rand() % (mMainMesh.number_of_faces() + 1);
	randnum = rand() % (mMainMesh.number_of_faces() + 1);
	randnum = rand() % (mMainMesh.number_of_faces() + 1);
	face_iterator iter = mMainMesh.faces_begin();
	std::advance(iter, randnum);
	return compute(iter);

}

ShapeContextUnit ShapeContext::compute(face_iterator fIter) {
	
	std::vector<Kernel::Point_3> faceVertices = getFaceVertices(fIter);
	Kernel::Point_3 f1Center = CGAL::centroid(faceVertices[0], faceVertices[1], faceVertices[2]);
	Kernel::Vector_3 f1Normal = CGAL::normal(faceVertices[0], faceVertices[1], faceVertices[2]);
	ShapeContextUnit scUnit = ShapeContextUnit(mMaxGeoVal, mMinGeoVal,f1Center);
	f1Normal = f1Normal / CGAL::sqrt(f1Normal[0] * f1Normal[0] + f1Normal[1] * f1Normal[1] + f1Normal[2] * f1Normal[2]);

	for (face_iterator iter = mMainMesh.faces_begin(); iter != mMainMesh.faces_end(); iter++) {
		if (iter == fIter) {
			scUnit.insert(agdFaceVector[*fIter], 0.0, Kernel::Vector_3(0,0,0),f1Center);
			continue;
		}
		std::vector<Kernel::Point_3> faceVecs = getFaceVertices(iter);
		Kernel::Point_3 f2Center = CGAL::centroid(faceVecs[0], faceVecs[1], faceVecs[2]);
		Kernel::Vector_3 f2fVector = f1Center - f2Center;
		f2fVector = f2fVector / CGAL::sqrt(f2fVector[0] * f2fVector[0] + f2fVector[1] * f2fVector[1] + f2fVector[2] * f2fVector[2]);
		double angle = computeAngle(f1Normal, f2fVector);
		scUnit.insert(agdFaceVector[*iter], angle, f2fVector,f2Center);
	}

	return scUnit;
}

void ShapeContext::generateFeatures(const char* fileName) {

	if (mMainMesh.is_empty()) return;

	std::ofstream curFile;

	curFile.open(fileName);
	if (!curFile)
		return;


	for (face_iterator iter = mMainMesh.faces_begin(); iter != mMainMesh.faces_end(); iter++) {
		ShapeContextUnit unit = compute(iter);
		curFile << (int)(*iter) << "\t";
		for (double features : unit.getShapeContextFeature()) {
			curFile << features << "\t";
		}
		curFile << "\n";
	}
	curFile.close();
}
