#ifndef  __SHAPE_CONTEXT_UNIT__
#define __SHAPE_CONTEXT_UNIT__
#define SC_GEO_BINS 5
#define SC_ANGLE_BINS 6
#define PI 3.14
#include <vector>
#include "GenericMesh.h"

class ShapeContextUnit {

private:
		double mSCBinMat[SC_GEO_BINS][SC_ANGLE_BINS];
		double gdIntervalBin[SC_GEO_BINS];
		double angleIntervalBin[SC_ANGLE_BINS];
		double mMaxGeoValue,mMinGeoValue;
		double mLogMaxGeoValue, mLogMinGeoValue;
		std::vector<Kernel::Vector_3> mSCPoints;
		Kernel::Point_3 faceNormal;
		std::vector<Kernel::Point_3> mNormalPoints;
	
public:
	ShapeContextUnit(double maxGeoValue,double minGeoValue,Kernel::Point_3 faceNormal) {
		
		for (int i = 0; i < SC_GEO_BINS; i++) {
			for (int j = 0; j < SC_ANGLE_BINS; j++) {
				mSCBinMat[i][j] = 0.0;
			}
		}
		
		mMaxGeoValue = maxGeoValue;
		mLogMaxGeoValue = log(mMaxGeoValue);
		mMinGeoValue = minGeoValue;
		mLogMinGeoValue = log(mMinGeoValue);
		
		double gdInterval = (mLogMaxGeoValue - mLogMinGeoValue) / SC_GEO_BINS;
		double angleInterval = (2 * PI) / SC_ANGLE_BINS;
		
		for (int i = 1; i <= SC_GEO_BINS; i++) {
			gdIntervalBin[i - 1] = gdInterval*i;
		}

		for (int i = 1; i <= SC_ANGLE_BINS; i++) {
			angleIntervalBin[i - 1] = angleInterval*i;
		}

		this->faceNormal = faceNormal;
	}

	void insert(double geoValue, double angle,Kernel::Vector_3 dirVec,Kernel::Point_3 normalPT) {
	
		double logGeo = log(geoValue);
		int i = 0, j = 0;

		for (; i < SC_GEO_BINS; i++) {
			if (gdIntervalBin[i] > geoValue) break;
		}
		for (; j < SC_ANGLE_BINS; j++) {
			if (angleIntervalBin[j] > angle) break;
		}	
		double scaledGeo;
		if (mLogMaxGeoValue != mLogMinGeoValue)
			scaledGeo = (logGeo - mLogMinGeoValue) / (mLogMaxGeoValue - mLogMinGeoValue);
		else
			scaledGeo = 1.0;
		mSCPoints.push_back(dirVec*scaledGeo);
		mSCBinMat[i][j] += logGeo;
		mNormalPoints.push_back(normalPT);
	}

	std::vector<Kernel::Vector_3> getSCPoints() {
		return mSCPoints;
	}

	std::vector<double> getShapeContextFeature() {
		std::vector<double> scFeatures;
		for (int i = 0; i < SC_GEO_BINS; i++) {
			for (int j = 0; j < SC_ANGLE_BINS; j++) {
				scFeatures.push_back(mSCBinMat[i][j]);
			}
		}
		return scFeatures;
	}


	std::vector<Kernel::Point_3> getAllFaceVector() {
		return mNormalPoints;
	}

	Kernel::Point_3 getFacePoint() {
		return faceNormal;
	}

};
#endif // ! __SHAPE_CONTEXT_UNIT__
