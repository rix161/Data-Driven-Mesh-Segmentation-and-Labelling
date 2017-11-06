#include "Curvature.h"
#include "CurvatureUnit.h"

Curvature::Curvature(const char* fileName) {
	std::ifstream in(fileName);
	in >> this->mMainMesh;
	std::cout<<mMainMesh.number_of_vertices() << std::endl;
	in.close();
}
void Curvature::computeBaryCenteric(Kernel::Point_3 facePoint, Kernel::Vector_3 faceNormal, std::vector<Kernel::Point_3> faceVertices, double &baryA, double &baryB, double &baryC) {
	
	double area1 = faceNormal * CGAL::cross_product(faceVertices[1] - faceVertices[0], faceVertices[2] - faceVertices[0]);
	double area2 = faceNormal * CGAL::cross_product(faceVertices[1] - facePoint, faceVertices[2] - facePoint);
	double area3 = faceNormal * CGAL::cross_product(faceVertices[2] - facePoint, faceVertices[0] - facePoint);

	baryA = area2 / area1;
	baryB = area3 / area1;
	baryC = 1.0 - baryA - baryB;
}

CurvatureUnit Curvature::computeFaceCurvature(face_iterator faceIter) {
	std::vector<Kernel::Point_3> faceVertices;
	std::vector<CurvatureUnit> results;
	double scale = 1.0;// 100000000000000;

	CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
	for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*faceIter), mMainMesh);
		vbegin != vend; vbegin++) {
		faceVertices.push_back(mMainMesh.point((*vbegin)));
	}

	Kernel::Point_3 faceCenter = CGAL::centroid(faceVertices[0], faceVertices[1], faceVertices[2]);
	Kernel::Vector_3 faceNormal = CGAL::normal(faceVertices[0], faceVertices[1], faceVertices[2]);
	Kernel::Vector_3 vAB = faceVertices[1] - faceVertices[0];
	Kernel::Vector_3 vBC = faceVertices[2] - faceVertices[1];
	Kernel::Vector_3 vCA = faceVertices[0] - faceVertices[2];

	Kernel::Vector_3 nA = CGAL::cross_product(vAB, vCA);
	Kernel::Vector_3 nB = CGAL::cross_product(vAB, vBC);
	Kernel::Vector_3 nC = CGAL::cross_product(vBC, vCA);
	
	double baryA, baryB, baryC;

	double area1 = faceNormal * CGAL::cross_product(faceVertices[1] - faceVertices[0], faceVertices[2] - faceVertices[0]);
	double area2 = faceNormal * CGAL::cross_product(faceVertices[1] - faceCenter, faceVertices[2] - faceCenter);
	double area3 = faceNormal * CGAL::cross_product(faceVertices[2] - faceCenter, faceVertices[0] - faceCenter);

	baryA = area2 / area1;
	baryB = area3 / area1;
	baryC = 1.0 - baryA - baryB;
	
	Kernel::Vector_3 nBar = baryA*nA + baryB*nB + baryC*nC;
	Kernel::Vector_3 mBar = CGAL::cross_product(faceVertices[1] - faceVertices[0], faceVertices[2] - faceVertices[1]);
	Kernel::Vector_3 hVec = CGAL::cross_product(nA, faceVertices[2] - faceVertices[1]) +
							CGAL::cross_product(nB, faceVertices[0] - faceVertices[2]) +
							CGAL::cross_product(nC, faceVertices[1] - faceVertices[0]);

	double gaussianCurve = CGAL::determinant(nA, nB, nC) / (nBar*nBar)*(nBar*mBar)*scale;
	double meanCurve	 = 0.5*(hVec*nBar/(CGAL::sqrt(nBar.squared_length())*(nBar*mBar)))*scale;
	double k1 = meanCurve + sqrt(meanCurve*meanCurve - gaussianCurve);
	double k2 = meanCurve - sqrt(meanCurve*meanCurve - gaussianCurve);

	double c1 = (nB - nA)*(faceVertices[1] - faceVertices[0]) / (faceVertices[1] - faceVertices[0]).squared_length();
	double c2 = (nC - nB)*(faceVertices[2] - faceVertices[1]) / (faceVertices[2] - faceVertices[1]).squared_length();
	double c3 = (nA - nC)*(faceVertices[0] - faceVertices[2]) / (faceVertices[0] - faceVertices[2]).squared_length();
	std::cout << " GC:" << gaussianCurve << " meanCurve:" << meanCurve << " k1:" << k1 << " k2:" << k2 << " c1:" << c1 << " c2:" << c2 << " c3:" << c3 << std::endl;

	CurvatureUnit cUnit(faceIter, gaussianCurve, meanCurve, k1, k2);
	return cUnit;
}

void initColorBins2(std::vector<std::vector<int>> &colorBins) {

	for (int i = 0; i < colorBins.size(); i++) {
		std::vector<int> color(4);
		color[0] = rand() % 255;
		color[1] = rand() % 255;
		color[2] = rand() % 255;
		color[3] = 0;
		colorBins[i] = color;
	}
}


void Curvature::computeMeshCurvature() {
	
	double min = 99999;
	double max = -99999;
	std::vector<CurvatureUnit> curves;

	int binCount = 25;
	std::vector<std::vector<int>> colorBin(binCount);
	initColorBins2(colorBin);

	for (face_iterator fit = mMainMesh.faces_begin(); fit != mMainMesh.faces_end(); fit++) {
		CurvatureUnit curve = computeFaceCurvature(fit);
		mCurves.push_back(curve);
		if (min > curve.getMCurve()) min = curve.getMCurve();
		if (max < curve.getMCurve()) max = curve.getMCurve();
	}
	double range = (max - min) / binCount;
	if (range == 0)
		range = 1;
	std::cout << "Max:" << max << "Min:" << min<<std::endl;
	int count = 0;
	for (CurvatureUnit unit : mCurves) {
		
		int binNum = unit.getMCurve() < 0 ? 0 : 1;
		std::vector<int> colorVec = colorBin[binNum];
		unit.setColor(colorVec);
		faceColor.push_back(colorVec);
	}

}



