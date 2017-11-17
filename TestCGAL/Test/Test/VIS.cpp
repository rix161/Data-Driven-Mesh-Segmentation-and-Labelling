#include "VIS.h"
#include "ShapeDiameterFunction.h"
#include <iostream>
#include <fstream>

std::vector<SDFUnit> VIS::computeMeshVIS(int rayCount) {
	std::vector<SDFUnit> rays;
	return rays;
};


void writeData(std::ofstream &sdfFile, std::vector<SDFUnit> &sdfFeatures) {
	double mean = 0, meansqr = 0, meansqrsqr = 0, meansqrt = 0, meansqrtsqrt = 0, median = 0;
	double alphas[4] = { 1.0f, 2.0f, 4.0f,8.0 };
	std::vector<double> values;
	for (SDFUnit unit : sdfFeatures) {
		double wtLength = unit.getWeightedLineLength();
		mean += wtLength;
		meansqr += (wtLength*wtLength);
		meansqrsqr += (wtLength*wtLength)*(wtLength*wtLength);
		meansqrt += sqrt(wtLength);
		meansqrtsqrt += sqrt(wtLength);
		values.push_back(wtLength);
	}
	std::sort(values.begin(), values.end());
	int vecSize = sdfFeatures.size();
	sdfFile << mean / (1.0*vecSize) << "\t"
		<< meansqr / (1.0*vecSize) << "\t"
		<< meansqrsqr / (1.0*vecSize) << "\t"
		<< meansqrt / (1.0*vecSize) << "\t"
		<< meansqrtsqrt / (1.0*vecSize) << "\t"
		<< values.at(values.size() / 2) << "\t";

	for (double alpha : alphas) {
		sdfFile << log((mean / (1.0*vecSize))*alpha + 1.0) / (log(alpha + 1.0)) << "\t"
			<< log((meansqr / (1.0*vecSize))*alpha + 1.0) / (log(alpha + 1.0)) << "\t"
			<< log((meansqrsqr / (1.0*vecSize))*alpha + 1.0) / (log(alpha + 1.0)) << "\t"
			<< log((meansqrt / (1.0*vecSize))*alpha + 1.0) / (log(alpha + 1.0)) << "\t"
			<< log((meansqrtsqrt / (1.0*vecSize))*alpha + 1.0) / (log(alpha + 1.0)) << "\t";
	}
}
void VIS::generateFeatures(const char* destinationFileSDF, const char* destinationFileVIS,int raycount) {

	int ANGLE_COUNT = 4;
	int LOG_ALPHA = 4;
	float angles[4] = { 15.0f,30.0f,45.0};
	float alphas[4] = { 1.0f, 2.0f, 4.0f,8.0 };

	if (mMainMesh.number_of_faces() > 6000)
		raycount = raycount/2;

	std::ofstream sdfFile,visFile;

	sdfFile.open(destinationFileSDF);
	if (!sdfFile)
		return;

	visFile.open(destinationFileVIS);
	if (!visFile)
		return;
	int count = 0;

	for (face_iterator fit = mMainMesh.faces_begin(); fit != mMainMesh.faces_end(); fit++) {
		if(count%100 ==0)
			std::cout << "Face:" << (int)(*fit) << std::endl;
		count++;
		sdfFile << (int)(*fit) << "\t";
		visFile << (int)(*fit) << "\t";
		for (float angle : angles) {
			std::pair<std::vector<SDFUnit>, std::vector<SDFUnit>> rawData = computeFaceVIS(fit, angle, raycount);
			writeData(sdfFile, rawData.first);
			writeData(visFile, rawData.second);
		}
		sdfFile << "\n";
		visFile << "\n";
	}
	sdfFile.close();
	visFile.close();

}

std::pair<std::vector<SDFUnit>, std::vector<SDFUnit>> VIS::computeFaceVIS(face_iterator face,float angle, int rayCount) {
	 std::vector<SDFUnit> rays = compute(face, angle, rayCount);
	 SDFUnit maxUnit;
	 double maxLine = -9999999;
	 for (SDFUnit &unit : rays) {
		 if (unit.getLineLength() > maxLine) {
			 maxLine = unit.getLineLength();
			 maxUnit = unit; 
		 }
	 }
	 Kernel::Point_3 midPoint = CGAL::midpoint(maxUnit.getEndPoint(), maxUnit.getStartPoint());
	 std::vector<SDFUnit> rays3 = computeIntersection(midPoint, Kernel::Vector_3(0.0, 1.0, 0.0), buildRays(glm::vec3(0.0, 1.0, 0.0), rayCount, 180, true),true);

#ifdef SHOW_MEDIAL_AXIS
	 maxUnit.setColor(glm::vec3(0, 0, 1.0));
	 rays3.push_back(maxUnit);
#endif

	 return std::pair<std::vector<SDFUnit>, std::vector<SDFUnit>>(rays, rays3);
}