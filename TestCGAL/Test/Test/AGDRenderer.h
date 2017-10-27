#ifndef __AGD_RENDERER__
#define __AGD_RENDERER__
#include "CGRenderer.h"
#include "ShaderHelper.h"
#include "GenericMesh.h"
#include <glm/glm.hpp>


class AGDRenderer{
private:
	GLuint mProgramId;

protected:
	void initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title);
	static void setupGlutCallBacks();

public:
	static bool AGDRenderer::mSetupDone;
	static GLuint AGDRenderer::mVBO;
	static int AGDRenderer::mVBOSize;
	static int AGDRenderer::vertexCount;
	static void renderScene();
	AGDRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title) {
		initRenderer(argc, argv, sizeX, sizeY, title);
	}

	void setupRendererParameters(
		const char* vertexShader,
		const char* fragmentShader,
		GenericMesh mesh);

	~AGDRenderer() {
		if (mProgramId != -1) {
			glDeleteProgram(mProgramId);
		}
	}
};
#endif