#ifndef __SDF_RENDERER__
#define __SDF_RENDERER__
#include "CGRenderer.h"
#include "ShaderHelper.h"
#include "GenericMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class SDFRenderer {
private:
	GLuint mProgramId;

protected:
	void initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title);
	static void setupGlutCallBacks();

public:
	static bool SDFRenderer::mSetupDone;

	static GLuint SDFRenderer::mVBO;
	static int SDFRenderer::mVBOSize;

	static int SDFRenderer::vertexCount;
	static GLuint SDFRenderer::uMVPMatrix;

	static int windowX, windowY;
	static float FOV, nearPlane, farPlane;

	static void renderScene();
	SDFRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title) {
		initRenderer(argc, argv, sizeX, sizeY, title);
	}

	void setupRendererParameters(
		const char* vertexShader,
		const char* fragmentShader,
		GenericMesh mesh,
		float alpha);

	~SDFRenderer() {
		if (mProgramId != -1) {
			glDeleteProgram(mProgramId);
		}
	}
};
#endif