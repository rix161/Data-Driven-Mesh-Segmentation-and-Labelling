#include <cstdlib>
#include <iostream>
#include <fstream>
#include "CGAL_geodesic.h"
#include "Edge_Dijkstra.h"
#include "Fast_Marching_Plane.h"

int main(int argc, char** argv)
{
	Triangle_mesh tmesh;

	std::ifstream in("F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\Models\\mushroom2.off");
	in >> tmesh;
	in.close();
	
	CGALGeodesic cgalGeo(tmesh);


	double avgDistace = cgalGeo.getAverageGeodesicDistance(tmesh.vertices().begin());
	std::cout << "AverageDistance: " << avgDistace<<std::endl;

	EdgeDijkstra edgeGeo(tmesh);
	double avgDistanceDjik = edgeGeo.getAverageGeodesicDistance(tmesh.vertices().begin());
	std::cout << "NaiveDjikstra: " << avgDistanceDjik << std::endl;

	FastMarchingPlanes fmPlane(tmesh);
	double avgDistancefMP = fmPlane.getAverageGeodesicDistance(tmesh.vertices().begin());
	std::cout << "FastMarching: " << avgDistancefMP << std::endl;
	fmPlane.compute();

	//getchar();
	return 0;
} 


/*#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

struct Vector3f
{
	float x;
	float y;
	float z;

	Vector3f() {}

	Vector3f(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f(const float* pFloat)
	{
		x = pFloat[0];
		y = pFloat[0];
		z = pFloat[0];
	}

	Vector3f(float f)
	{
		x = y = z = f;
	}

	Vector3f& operator+=(const Vector3f& r)
	{
		x += r.x;
		y += r.y;
		z += r.z;

		return *this;
	}

	Vector3f& operator-=(const Vector3f& r)
	{
		x -= r.x;
		y -= r.y;
		z -= r.z;

		return *this;
	}

	Vector3f& operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	operator const float*() const
	{
		return &(x);
	}


	Vector3f Cross(const Vector3f& v) const;

	Vector3f& Normalize();

	void Rotate(float Angle, const Vector3f& Axis);

	void Print() const
	{
		printf("(%.02f, %.02f, %.02f)", x, y, z);
	}
};

GLuint VBO;

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_POINTS, 0, 1);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}


static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
}


static void CreateVertexBuffer()
{
	Vector3f Vertices[1];
	Vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 02");

	InitializeGlutCallbacks();

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer();

	glutMainLoop();

	return 0;
}*/