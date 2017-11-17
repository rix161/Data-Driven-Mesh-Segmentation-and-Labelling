#include "Curvature.h"
#include "CurvatureUnit.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

Curvature::Curvature(const char* fileName) {
	std::ifstream in(fileName);
	in >> this->mMainMesh;
	std::cout<<mMainMesh.number_of_vertices() << std::endl;
	in.close();
	this->setInterpolate(true);
}


std::vector<CurvatureUnit> Curvature::computeMeshCurvature2() {

	//TBD!!!!!
	std::vector<double> areas(mMainMesh.number_of_vertices());
	std::vector<Kernel::Vector_3> coord(mMainMesh.number_of_vertices());
	std::vector<Kernel::Vector_3> normals(mMainMesh.number_of_vertices());
	std::vector<double> kMean(mMainMesh.number_of_vertices());
	std::vector<double> kGauss(mMainMesh.number_of_vertices());
	std::map<Kernel::Point_3,CGAL::SM_Vertex_index> vertexMap;
	std::vector<CurvatureUnit> results;

	double PI2 = 3.14;
	for (int i = 0; i < mMainMesh.number_of_vertices(); i++) {
		areas[i] = 0.0;
		kMean[i] = 0.0;
		kGauss[i] = PI2*2;
		coord[i] = Kernel::Vector_3(0, 0, 0);
	}

	for (face_iterator fIter = mMainMesh.faces_begin(); fIter != mMainMesh.faces_end(); fIter++) {
		std::vector<Kernel::Point_3> faceVertices;
		float angle0, angle1, angle2;

		CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
		for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*fIter), mMainMesh);
			vbegin != vend; vbegin++) {
			faceVertices.push_back(mMainMesh.point((*vbegin)));
			vertexMap.insert(std::pair<Kernel::Point_3, CGAL::SM_Vertex_index>(mMainMesh.point(*vbegin), (*vbegin)));
		}

		Kernel::Vector_3 vec1 = faceVertices[1] - faceVertices[0];
		Kernel::Vector_3 vec2 = faceVertices[2] - faceVertices[0];
		Kernel::Vector_3 vec3 = faceVertices[0] - faceVertices[1];
		Kernel::Vector_3 vec4 = faceVertices[2] - faceVertices[1];

		angle0 = glm::angle(glm::normalize(glm::vec3(vec1[0], vec1[1], vec1[2])), glm::normalize(glm::vec3(vec2[0], vec2[1], vec2[2])));
		angle1 = glm::angle(glm::normalize(glm::vec3(vec3[0], vec3[1], vec3[2])), glm::normalize(glm::vec3(vec4[0], vec4[1], vec4[2])));
		angle2 = PI2 - (angle0 + angle1);
		if (CGAL::collinear(faceVertices[0], faceVertices[1], faceVertices[2])) {
			std::cout << "V1:" << faceVertices[0] << " v2:" << faceVertices[1] << " v3:" << faceVertices[2] << "Are collinear" << std::endl;
			results.push_back(CurvatureUnit(fIter, 0, 0, 0, 0));
			continue;
		}
		Kernel::Vector_3 normal = CGAL::normal(faceVertices[0], faceVertices[1], faceVertices[2]);
		normals[vertexMap[faceVertices[0]]] = normal;
		normals[vertexMap[faceVertices[1]]] = normal;
		normals[vertexMap[faceVertices[2]]] = normal;

		if (angle0 < (PI2 / 2) && angle1 < (PI2 / 2) && angle2 < (PI2 / 2)) {
			double e0 = CGAL::squared_distance(faceVertices[1], faceVertices[0]);
			double e1 = CGAL::squared_distance(faceVertices[2], faceVertices[1]);
			double e2= CGAL::squared_distance(faceVertices[0], faceVertices[2]);

			areas[vertexMap[faceVertices[0]]] += (e2*(1 / tan(angle1)) + e0*(1 / tan(angle2))) / 8.0;
			areas[vertexMap[faceVertices[1]]] += (e0*(1 / tan(angle2)) + e1*(1 / tan(angle0))) / 8.0;
			areas[vertexMap[faceVertices[2]]] += (e1*(1 / tan(angle0)) + e2*(1 / tan(angle1))) / 8.0;

		}
		else {
			double area = sqrt(CGAL::squared_area(faceVertices[0], faceVertices[1], faceVertices[2]));
			if (angle0 >= (PI2 / 2)) {
				areas[vertexMap[faceVertices[0]]] += area / 2;
				areas[vertexMap[faceVertices[1]]] += area / 4;
				areas[vertexMap[faceVertices[2]]] += area / 4;
			
			}
			else if(angle1 >= (PI2 / 2)){
				areas[vertexMap[faceVertices[0]]] += area / 4;
				areas[vertexMap[faceVertices[1]]] += area / 2;
				areas[vertexMap[faceVertices[2]]] += area / 4;
			}
			else {
				areas[vertexMap[faceVertices[0]]] += area / 4;
				areas[vertexMap[faceVertices[1]]] += area / 4;
				areas[vertexMap[faceVertices[2]]] += area / 2;
			}
		}
	}


	for (face_iterator fIter = mMainMesh.faces_begin(); fIter != mMainMesh.faces_end(); fIter++) {
		std::vector<Kernel::Point_3> faceVertices;
		float angle0, angle1, angle2;

		CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
		for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*fIter), mMainMesh);
			vbegin != vend; vbegin++) {
			faceVertices.push_back(mMainMesh.point((*vbegin)));
		}
		Kernel::Vector_3 vec1 = faceVertices[1] - faceVertices[0];
		Kernel::Vector_3 vec2 = faceVertices[2] - faceVertices[0];
		Kernel::Vector_3 vec3 = faceVertices[0] - faceVertices[1];
		Kernel::Vector_3 vec4 = faceVertices[2] - faceVertices[1];

		angle0 = glm::angle(glm::normalize(glm::vec3(vec1[0], vec1[1], vec1[2])), glm::normalize(glm::vec3(vec2[0], vec2[1], vec2[2])));
		angle1 = glm::angle(glm::normalize(glm::vec3(vec3[0], vec3[1], vec3[2])), glm::normalize(glm::vec3(vec4[0], vec4[1], vec4[2])));
		angle2 = PI2 - (angle0 + angle1);

		Kernel::Vector_3 e0 = faceVertices[1] - faceVertices[0];
		Kernel::Vector_3 e1 = faceVertices[2] - faceVertices[1];
		Kernel::Vector_3 e2 = faceVertices[0] - faceVertices[2];
	
		if (angle0 == 0 || angle1 == 0 || angle2 == 0) continue;

		coord[vertexMap[faceVertices[0]]] += (e2*(1 / tan(angle1)) - e0*(1 / tan(angle2))) / 4.0;
		coord[vertexMap[faceVertices[1]]] += (e0*(1 / tan(angle2)) - e1*(1 / tan(angle0))) / 4.0;
		coord[vertexMap[faceVertices[2]]] += (e1*(1 / tan(angle0)) - e2*(1 / tan(angle1))) / 4.0;

		kGauss[vertexMap[faceVertices[0]]] -= angle0;
		kGauss[vertexMap[faceVertices[1]]] -= angle1;
		kGauss[vertexMap[faceVertices[2]]] -= angle2;

		CGAL::Edge_around_face_iterator<Triangle_mesh> hbegin,hend;
		for (boost::tie(hbegin, hend) = CGAL::edges_around_face(mMainMesh.halfedge(*fIter), mMainMesh);
			hbegin != hend; hbegin++) {

			if (mMainMesh.is_border(*hbegin)) {
				Kernel::Vector_3 p1 = mMainMesh.point(mMainMesh.vertex(*hbegin, 1)) - mMainMesh.point(mMainMesh.vertex(*hbegin, 0));
				hbegin++;
				Kernel::Vector_3 p2 = mMainMesh.point(mMainMesh.vertex(*hbegin, 1)) - mMainMesh.point(mMainMesh.vertex(*hbegin, 0));
				kGauss[vertexMap[mMainMesh.point(mMainMesh.vertex(*hbegin, 0))]] -= p1*p2;
				kGauss[vertexMap[mMainMesh.point(mMainMesh.vertex(*hbegin, 1))]] -= p1*p2;
			}
		}
	}

	for (vertex_iterator vi = mMainMesh.vertices_begin(); vi != mMainMesh.vertices_end(); vi++) {
		
		Kernel::Point_3 p = mMainMesh.point(*vi);
		int value = vertexMap[p];
		kMean[value] = ((normals[value] * coord[value]) > 0 ? 1.0 : -1) * sqrt((coord[value]/areas[value]).squared_length());
		kGauss[value] /= areas[value];
	
	}
	double min = 99999999, max = -999999;
	for (face_iterator fIter = mMainMesh.faces_begin(); fIter != mMainMesh.faces_end(); fIter++) {
		std::vector<Kernel::Point_3> faceVertices;
		int count = 0;
		double mean = 0;
		double gauss = 0;
		CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
		for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*fIter), mMainMesh);
			vbegin != vend; vbegin++) {
			count++;
			if (kGauss[*vbegin] < min) min = kGauss[*vbegin];
			if (kGauss[*vbegin] > max) max = kGauss[*vbegin];
			mean += kMean[*vbegin];
			gauss += kGauss[*vbegin];
		}
		double meanCurve = mean / count;
		double gaussianCurve = gauss / count;
		double val = meanCurve*meanCurve - gaussianCurve;
		if (val < 0)
			val = 0;
		double k1 = meanCurve + sqrt(meanCurve*meanCurve - gaussianCurve);
		double k2 = meanCurve - sqrt(meanCurve*meanCurve - gaussianCurve);
		results.push_back(CurvatureUnit(fIter, gaussianCurve, meanCurve, k1, k2));
	}

#ifdef DEBUG_MODE
	std::cout << "Min_PV:" << min << " Max_PV:" << max << std::endl;
	for (double kMeanVal : kMean)
		std::cout << kMeanVal << "\t";
	std::cout << "\n";
	
	for (double kGaussVal : kGauss)
		std::cout << kGaussVal << "\t";
	std::cout << "\n";
#endif
	return results;
}


void Curvature::computeBaryCenteric(Kernel::Point_3 facePoint, Kernel::Vector_3 faceNormal, std::vector<Kernel::Point_3> faceVertices, double &baryA, double &baryB, double &baryC) {
	
	double area1 = faceNormal * CGAL::cross_product(faceVertices[1] - faceVertices[0], faceVertices[2] - faceVertices[0]);
	double area2 = faceNormal * CGAL::cross_product(faceVertices[1] - facePoint, faceVertices[2] - facePoint);
	double area3 = faceNormal * CGAL::cross_product(faceVertices[2] - facePoint, faceVertices[0] - facePoint);

	baryA = area2 / area1;
	baryB = area3 / area1;
	baryC = 1.0 - baryA - baryB;
}

void Curvature::buildNRings(face_iterator fIter, std::set<Kernel::Point_3> &nRing, int n) {
	if (n == 0)
		return;

	CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
	for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*fIter), mMainMesh);
		vbegin != vend; vbegin++) {
		nRing.insert(mMainMesh.point((*vbegin)));
	}

	CGAL::Face_around_face_iterator<Triangle_mesh> fbegin, fend;
	for (boost::tie(fbegin, fend) = CGAL::faces_around_face(mMainMesh.halfedge(*fIter), mMainMesh);
		fbegin != fend; fbegin++) {
		face_iterator iter = mMainMesh.faces_begin();
		std::advance(iter, (*fbegin));
		buildNRings(iter, nRing, n - 1);
	}
}


void Curvature::buildNRings(face_iterator fIter, std::set<Triangle_mesh::Face_index> &nRing, int n) {
	if (n == 0)
		return;

	CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
	for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*fIter), mMainMesh);
		vbegin != vend; vbegin++) {
		nRing.insert(mMainMesh.face(mMainMesh.halfedge((*vbegin))));
	}

	CGAL::Face_around_face_iterator<Triangle_mesh> fbegin, fend;
	for (boost::tie(fbegin, fend) = CGAL::faces_around_face(mMainMesh.halfedge(*fIter), mMainMesh);
		fbegin != fend; fbegin++) {
		face_iterator iter = mMainMesh.faces_begin();
		std::advance(iter, (*fbegin));
		buildNRings(iter, nRing, n - 1);
	}
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

	Kernel::Vector_3 nA = (CGAL::cross_product(vAB, vCA));
	nA = nA / sqrt(nA.squared_length());
	Kernel::Vector_3 nB = CGAL::cross_product(vAB, vBC);
	nB = nB / sqrt(nA.squared_length());
	Kernel::Vector_3 nC = CGAL::cross_product(vBC, vCA);
	nC = nC / sqrt(nA.squared_length());
	
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
	std::cout << " M1: GC:" << gaussianCurve << " meanCurve:" << meanCurve << " k1:" << k1 << " k2:" << k2 << " c1:" << c1 << " c2:" << c2 << " c3:" << c3 << std::endl;

	///Method2 
	
	/*faceVertices.clear();
	std::set<Kernel::Point_3> NRing;

	buildNRings(faceIter, NRing, 2);
	Eigen::MatrixXd  P(NRing.size(), 3);
	int i = 0;
	for (Kernel::Point_3 point : NRing) {
		P(i, 0) = point.x();
		P(i, 1) = point.y();
		P(i, 2) = point.z();
		i++;
	}
	
	Eigen::MatrixXd  PT = P.transpose();
	Eigen::MatrixXd temp = P*PT;
	Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(temp);
	if (eigensolver.info() != Eigen::Success) std::cout<<"ERROR";
	Eigen::VectorXd eVal= eigensolver.eigenvalues();
	Eigen::MatrixXd eVecs = eigensolver.eigenvectors();
	std::cout << "eVal:" << eVal << std::endl;
	std::cout << "eVecs:" << eVecs << std::endl;*/


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

	/*for (face_iterator fit = mMainMesh.faces_begin(); fit != mMainMesh.faces_end(); fit++) {
		CurvatureUnit curve = computeFaceCurvature(fit);
		mCurves.push_back(curve);
		if (min > curve.getMCurve()) min = curve.getMCurve();
		if (max < curve.getMCurve()) max = curve.getMCurve();
	}*/

	mCurves = computeMeshCurvature2();
	for (CurvatureUnit curve : mCurves) {
		if (min > curve.getGCurve()) min = curve.getGCurve();
		if (max < curve.getGCurve()) max = curve.getGCurve();
	}

	std::cout << "Max:" << max << "Min:" << min<<std::endl;
	
	for (CurvatureUnit &unit : mCurves) {
		float scale = (unit.getGCurve() - min) / (max - min);

		int binNum = floor((unit.getGCurve() - min) / (max - min));
		std::vector<int> colorVec = colorBin[binNum];

		unit.setCurveScale(scale);
		faceColorScales.push_back(scale);
		faceColor.push_back(colorVec);
	}

}


void Curvature::generateFeatures(const char* fileName) {
	

	std::ofstream curFile;

	curFile.open(fileName);
	if (!curFile)
		return;

	int ringCount[5] = {0,1,2,4};
	std::map<Triangle_mesh::Face_index, CurvatureUnit> cUnitMap;
	std::vector<CurvatureUnit> units = computeMeshCurvature2();
	for (CurvatureUnit unit : units)
		cUnitMap[(*unit.getFaceIter())] = unit;
	
	for (face_iterator fbegin = mMainMesh.faces().begin(); fbegin != mMainMesh.faces().end(); fbegin++) {
		curFile << (int)(*fbegin) << "\t";
		for (int rCount : ringCount) {
			std::set<Triangle_mesh::Face_index> nRing;
			nRing.insert(*fbegin);
			buildNRings(fbegin, nRing, rCount);
			double meanMeanCurve = 0.0;
			double meanGaussianCurve = 0.0;
			for (Triangle_mesh::Face_index index : nRing) {
				meanMeanCurve += cUnitMap.at(index).getMCurve();
				meanGaussianCurve += cUnitMap.at(index).getMCurve();
			}
			meanMeanCurve /= nRing.size();
			meanGaussianCurve /= nRing.size();
			double val = meanMeanCurve*meanMeanCurve - meanGaussianCurve;
			if (val < 0)
				val = 0;
			double k1 = meanMeanCurve + sqrt(val);
			double k2 = meanMeanCurve - sqrt(val);
			curFile <<k1 << "\t"
				<< abs(k1) << "\t"
				<< k2 << "\t"
				<< abs(k2) << "\t"
				<< k1*k2 << "\t"
				<< abs(k1*k2) << "\t"
				<< (k1 + k2) / 2.0 << "\t"
				<< abs((k1 + k2) / 2.0) << "\t"
				<< k1 - k2;
		}
		curFile << "\n";
	}


}



