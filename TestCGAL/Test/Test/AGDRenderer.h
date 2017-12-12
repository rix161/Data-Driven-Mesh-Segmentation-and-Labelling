#ifndef __AGD_RENDERER__
#define __AGD_RENDERER__
#include "ShaderHelper.h"
#include "GenericMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class AGDRenderer{
private:
	GLuint mProgramId;

protected:
	void initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title);
	static void setupGlutCallBacks();

public:
	static bool AGDRenderer::mSetupDone;

	static GLuint AGDRenderer::mVBO;
	static GLuint AGDRenderer::mVBO2;
	static int AGDRenderer::mVBOSize;

	static int AGDRenderer::vertexCount;
	static GLuint AGDRenderer::uMVPMatrix;

	static int windowX, windowY;
	static float FOV, nearPlane, farPlane;
	static float mTX, mTY,mTZ;
	GLuint getRenderedTexture();

	static void renderScene();
	
	void setupSecondMesh(GenericMesh mesh);
	void renderSceneDouble();

	AGDRenderer() { AGDRenderer::mVBO = 0; };
	AGDRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title) {
		initRenderer(argc, argv, sizeX, sizeY, title);
	}

	void setWindow(int windowX, int windowY) {
		this->windowX = windowX;
		this->windowY = windowY;
	}

	void updateCamera(float tVal[]) {
		mTX += tVal[0];
		mTY += tVal[1];
		mTZ += tVal[2];
	}
	void setupRendererParameters(
		const char* vertexShader,
		const char* fragmentShader,
		GenericMesh mesh);

	~AGDRenderer() {
		if (mProgramId != 0) {
			glDeleteProgram(mProgramId);
		}

		if(mVBO!=0)
			glDeleteBuffers(1, &AGDRenderer::mVBO);
		if (mVBO2 != 0)
			glDeleteBuffers(1, &AGDRenderer::mVBO2);
	}
};
#endif