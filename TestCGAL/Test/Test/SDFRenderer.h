#ifndef __SDF_RENDERER__
#define __SDF_RENDERER__
#include "CGRenderer.h"
#include "ShaderHelper.h"
#include "GenericMesh.h"
#include "SDFUnit.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class SDFRenderer {
private:
	static GLuint mProgramId;
	static GLuint mRayProgramId;

protected:
	void initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title);
	static void setupGlutCallBacks();

public:

	static bool SDFRenderer::mSetupDone;

	static GLuint SDFRenderer::mVBO;
	static GLuint SDFRenderer::mVBO2;
	static int SDFRenderer::mVBOSize;

	static int SDFRenderer::vertexCount;
	static int SDFRenderer::rayCount;
	static GLuint SDFRenderer::uMVPMatrix;
	static GLuint SDFRenderer::uMVPMatrix2;

	static int windowX, windowY;
	static float FOV, nearPlane, farPlane;

	static void renderScene();
	SDFRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title) {
		initRenderer(argc, argv, sizeX, sizeY, title);
	}

	void setupRendererParameters(
		const char* vertexShader,
		const char* fragmentShader,
		const char* rayVertexShader,
		const char* rayFragmentShader,
		GenericMesh mesh,
		std::vector<SDFUnit> rays);

	~SDFRenderer() {
		if (mProgramId != -1) {
			glDeleteProgram(mProgramId);
		}
		if (mRayProgramId != -1) {
			glDeleteProgram(mRayProgramId);
		}
	}
};
#endif