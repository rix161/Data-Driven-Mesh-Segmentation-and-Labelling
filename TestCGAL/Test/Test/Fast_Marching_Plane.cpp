#include "Fast_Marching_Plane.h"

#include <CGAL/Linear_algebraCd.h>
#include <CGAL/Linear_algebraHd.h>
#include <CGAL/enum.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "Off_Writer.h"

typedef CGAL::Linear_algebraCd<Kernel::FT> Linear_op;
typedef Kernel::Vector_3                        Value;
typedef std::vector<Value>                      Vector;
typedef Vector::iterator                        Value_iterator;
typedef std::vector<Vector>                     Vector_cont;
typedef CGAL::Linear_algebraCd<Kernel::FT>::Matrix Matrix;
typedef CGAL::Linear_algebraCd<Kernel::FT>::Vector LAVector;

void initColorBins(std::vector<std::vector<int>> &colorBins) {

	for (int i = 0; i < colorBins.size(); i++) {
		std::vector<int> color(4);
		color[0] = rand() % 255;
		color[1] = rand() % 255;
		color[2] = rand() % 255;
		color[3] = 0;
		colorBins[i] = color;
	}
}

void FastMarchingPlanes::compute() {

	std::map<Kernel::Point_3, double> agdMap;
	std::map<Kernel::Point_3, double> faceAgdMap;
	int binCount = 5;
	double min = 9999;
	double max = -9999;
	OffWriter writter;

	std::vector<Kernel::Point_3>vertexIndex;
	std::vector<std::vector<int>>vectexColor;
	std::vector<std::deque<int>>faceIndex;
	std::vector<std::vector<int>>faceColor;

	for (vertex_iterator it = mMainMesh.vertices().begin(); it != mMainMesh.vertices().end(); it++) {
		double agdDist = getAverageGeodesicDistance(it);
		Kernel::Point_3 pt = mMainMesh.point(*it);
		vertexIndex.push_back(pt);
		agdMap[pt] = agdDist;
		if (agdDist > max) max = agdDist;
		if (agdDist < min) min = agdDist;
	}

	double range = (max - min) / binCount;
	std::vector<std::vector<int>> colorBin(binCount);
	initColorBins(colorBin);

	for (face_iterator fit = mMainMesh.faces().begin(); fit != mMainMesh.faces().end(); fit++) {
		CGAL::Vertex_around_face_iterator<Triangle_mesh> vbegin, vend;
		double avgGd = 0.0;
		int count = 0;
		std::deque<int> vertexIndex;
		for (boost::tie(vbegin, vend) = CGAL::vertices_around_face(mMainMesh.halfedge(*fit), mMainMesh);
			vbegin != vend; vbegin++) {
			vertexIndex.push_back((int)(*vbegin));
			avgGd += agdMap[mMainMesh.point(*vbegin)];
			count++;
		}
		vertexIndex.push_front(count);
		faceIndex.push_back(vertexIndex);

		avgGd /= count;
		int binNum = floor((avgGd - min) / range);
		std::vector<int> colorVec = colorBin[binNum];
		faceColor.push_back(colorVec);
	}

	writter.writeFile("F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\output\\output.off",
		vertexIndex,
		vectexColor,
		faceIndex,
		faceColor,
		mMainMesh.number_of_vertices(),
		mMainMesh.number_of_faces(),
		mMainMesh.number_of_edges());

}


Matrix init_matrix(const int m, const int n, Kernel::FT entries[]) {

	Matrix mat(m, n);
	int i, j;
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			mat[i][j] = entries[i*m + j];

	return mat;
} // end initialization of matrix

void printMatix(const Matrix Mat) {

	for (int i = 0; i < Mat.row_dimension(); i++) {
		for (int j = 0; j < Mat.column_dimension(); j++) {
			std::cout << Mat[i][j] << "\t";
		}
		std::cout << std::endl;
	}

}

double propagatePlane2(double Ta, double Tb, double Tc, double La, double Lb, double angle) {
	double u = fabs(Ta - Tb);
	double T1 = Ta*Ta + Tb*Tb - 2 * Ta*Tb*cos(angle);
	double T2 = 2 * Tb*u*(Ta*cos(angle) - Tb);
	double T3 = Tb*Tb*(u*u - (Ta*Ta*sin(angle)*sin(angle)));
	double TcDash1 = (-T2 + sqrt((T2*T2) - (4 * T1*T3))) / (2 * T1);
	double TcDash2 = (-T2 - sqrt((T2*T2) - (4 * T1*T3))) / (2 * T1);
	double TcDash = TcDash1 > TcDash2 ? TcDash1 : TcDash2;
	double Tempcheck = (Tb*(TcDash - u)) / TcDash;
	bool check1 = u < TcDash;
	bool check2 = (Ta*cos(angle)) < (Tempcheck);
	bool check3 = (Tempcheck) < (Ta / cos(angle));

	if (check1 && check2 && check3) {
		Tc = Tc < (TcDash + Ta) ? Tc : TcDash + Ta;
	}
	else {
		double min1 = Tc < (La + Ta) ? Tc : (La + Ta);
		Tc = min1 < (Lb + Tb) ? min1 : (Lb + Tb);
	}

	return Tc;
}

double propagatePlane(Kernel::Point_3 p1, Kernel::Point_3 p2, Kernel::Point_3 p3, double d1, double d2, double d3) {

	Kernel::Vector_3 A = p1 - p3;
	Kernel::Vector_3 B = p2 - p3;
	CGAL::Angle ang = CGAL::angle(A, B);
	std::cout << "Angle:" << ang << std::endl;

	double Asqr = A*A;
	double Bsqr = B*B;
	double AB = A*B;
	double BA = B*A;
	double det = Asqr*Bsqr - (AB*BA);

	Kernel::FT qVec[4] = { Bsqr / det, -BA / det,-AB / det,Asqr / det };
	Kernel::FT dVec[2] = { d1,d2 };
	Kernel::FT dVec2[2] = { 1,1 };

	Matrix Q = init_matrix(2, 2, qVec);
	Matrix DTranspose = init_matrix(1, 2, dVec);
	Matrix D = CGAL::Linear_algebraCd<Kernel::FT>::transpose(DTranspose);
	Matrix One = init_matrix(1, 2, dVec2);
	Matrix OneTranspose = CGAL::Linear_algebraCd<Kernel::FT>::transpose(One);

	Matrix QD = Q*D;
	Matrix InterMat1 = One*(QD);
	Matrix InterMat2 = DTranspose*QD;
	Matrix InterMat3 = One*(Q*OneTranspose);
	Matrix InterMat4 = (InterMat1*InterMat1 - (InterMat3)*(InterMat2));
	double p = (InterMat1[0][0] + sqrt(InterMat4[0][0])) / InterMat3[0][0];
	std::cout << "P:" << p << std::endl;
	Matrix n = Q*(D - p*OneTranspose);
	printMatix(D);
	printMatix(n);

	if (ang == CGAL::Angle::ACUTE || ang == CGAL::Angle::RIGHT) {
		return (p < d3) ? p : d3;
	}
	else {
		double v1 = d1 + sqrt(A.squared_length());
		double v2 = d2 + sqrt(A.squared_length());
		double v3 = d3;

		if (v1 < v2 && v1 < v3) return v1;
		else if (v2 < v1 && v2 < v3) return v2;
		else return v3;
	}

}

/*double FastMarchingPlanes::getAverageGeodesicDistance(vertex_iterator sourcePoint) {


if (mMainMesh.is_empty()) return 0.0;

DjiElement source(sourcePoint, mMainMesh.point(*sourcePoint), 0.0);
std::priority_queue < DjiElement, std::vector<DjiElement>, compare> minQueue;
std::set<Kernel::Point_3> visitedSet;
std::map<Kernel::Point_3, double> vertexMap;
vertex_iterator lvit, lvit_end;
for (boost::tie(lvit, lvit_end) = vertices(mMainMesh); lvit != lvit_end; ++lvit) {
vertexMap[mMainMesh.point(*lvit)] = 999999.9;
}

vertexMap[mMainMesh.point(*source.getVertex())] = 0.0;
minQueue.push(source);

while (!minQueue.empty()) {

DjiElement initElement = minQueue.top();
minQueue.pop();

if (visitedSet.find(initElement.getPoint()) != visitedSet.end()) continue;

visitedSet.insert(initElement.getPoint());

std::vector<DjiElement> neighbours = getNeighbors(initElement.getVertex(), vertexMap);
Kernel::Point_3 pPoint = mMainMesh.point(*initElement.getVertex());

propagatePlane(pPoint, mMainMesh.point(*neighbours.at(0).getVertex()), mMainMesh.point(*neighbours.at(1).getVertex()),1.0,1.0);
}
return 0;
}*/


void FastMarchingPlanes::computeUpdate(
	DjiElement trial,
	std::map<Kernel::Point_3, double> &vertexMap,
	UpdatablePriorityQueue < DjiElement, std::vector<DjiElement>, compare> &minQueue,
	std::set<Kernel::Point_3> fixedSet,
	std::set<Kernel::Point_3> &closedSet,
	std::set<Kernel::Point_3> &unProcessedSet) {

	std::vector<DjiElement> trialNeig = getNeighbors(trial.getVertex(), vertexMap);

	for (DjiElement ele : trialNeig) {

		if (fixedSet.count(ele.getPoint()) != 0) continue;

		std::vector<DjiElement> testNeig = getNeighbors(ele.getVertex(), vertexMap);
		for (DjiElement testEle : testNeig) {

			if (fixedSet.count(testEle.getPoint()) != 0) {

				if (unProcessedSet.count(ele.getPoint()) > 0) {
					closedSet.insert(ele.getPoint());
					unProcessedSet.erase(ele.getPoint());
				}

				double Ta = vertexMap[trial.getPoint()];
				double Tb = vertexMap[testEle.getPoint()];
				double Tc = vertexMap[ele.getPoint()];
				double La = sqrt(CGAL::squared_distance(trial.getPoint(), ele.getPoint()));
				double Lb = sqrt(CGAL::squared_distance(testEle.getPoint(), ele.getPoint()));

				Kernel::Vector_3 v1 = trial.getPoint() - ele.getPoint();
				v1 = v1 / std::sqrt(v1 * v1);
				Kernel::Vector_3 v2 = testEle.getPoint() - ele.getPoint();
				v2 = v2 / std::sqrt(v2 * v2);

				double angle1 = std::acos(v1 * v2) / CGAL_PI * 180;
				CGAL::Angle angle = CGAL::angle(trial.getPoint() - testEle.getPoint(), ele.getPoint() - testEle.getPoint());
				double p = propagatePlane2(Ta, Tb, Tc, La, Lb, angle1);
				if (vertexMap.at(ele.getPoint()) > p) {
					vertexMap.at(ele.getPoint()) = p;
					minQueue.updatePriority(ele, p);
				}
			}
		}
	}
}

double FastMarchingPlanes::getAverageGeodesicDistance(vertex_iterator sourcePoint) {

	std::set<Kernel::Point_3> fixedSet;
	std::set<Kernel::Point_3> closeSet;
	std::set<Kernel::Point_3> unprocessedSet;
	std::map<Kernel::Point_3, double> vertexMap;
	UpdatablePriorityQueue < DjiElement, std::vector<DjiElement>, compare> minQueue;

	vertex_iterator lvit, lvit_end;
	for (boost::tie(lvit, lvit_end) = vertices(mMainMesh); lvit != lvit_end; ++lvit) {
		vertexMap[mMainMesh.point(*lvit)] = 999999.9;
		unprocessedSet.insert(mMainMesh.point(*lvit));
	}

	DjiElement source(sourcePoint, mMainMesh.point(*sourcePoint), 0.0);
	vertexMap[mMainMesh.point(*source.getVertex())] = 0.0;
	unprocessedSet.erase(source.getPoint());
	fixedSet.insert(source.getPoint());

	std::vector<DjiElement> neighbours = getNeighbors(source.getVertex(), vertexMap);
	for (DjiElement ele : neighbours) {
		closeSet.insert(ele.getPoint());
		unprocessedSet.erase(ele.getPoint());

		vertexMap[ele.getPoint()] = sqrt(CGAL::squared_distance(ele.getPoint(), source.getPoint()));
		ele.updateDistance(vertexMap[ele.getPoint()]);
		minQueue.push(ele);
	}


	while (!minQueue.empty()) {

		DjiElement trial = minQueue.top();

		if (fixedSet.count(trial.getPoint()) != 0) continue;

		minQueue.pop();
		closeSet.erase(trial.getPoint());
		computeUpdate(trial, vertexMap, minQueue, fixedSet, closeSet, unprocessedSet);
		fixedSet.insert(trial.getPoint());

	}
	double sum = 0;
	for (std::map<Kernel::Point_3, double>::iterator it = vertexMap.begin(); it != vertexMap.end(); it++) {
		sum += (*it).second;
	}

	return sum / vertexMap.size();

}
