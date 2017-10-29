#ifndef __CGRENDERER__
#define __CGRENDERER__
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "CGAL_geodesic.h"
#include "ShaderHelper.h"
#include "GenericMesh.h"

class CGRenderer {
private:
	GLuint mProgramId;

protected:
	void initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title);
	static bool CGRenderer::mSetupDone;

	static GLuint CGRenderer::mVBO;
	static int CGRenderer::mVBOSize;

	static int CGRenderer::vertexCount;
	static GLuint CGRenderer::uMVPMatrix;

	static int windowX, windowY;
	static float FOV, nearPlane, farPlane;
public:
	CGRenderer() {}
	CGRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title){
		initRenderer(argc, argv, sizeX, sizeY, title);
	}
	
	void setupRendererParameters(
		const char* vertexShader,
		const char* fragmentShader,
		GenericMesh mesh,
		float alpha);
	GLuint getProgramId() { return mProgramId; }

	~CGRenderer() {
		if (mProgramId != -1) {
			glDeleteProgram(mProgramId);
		}
	}

};
#endif
