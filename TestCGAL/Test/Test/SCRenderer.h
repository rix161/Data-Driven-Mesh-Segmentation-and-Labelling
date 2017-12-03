#ifndef __SC_RENDERER__
#define __SC_RENDERER__


#include "ShaderHelper.h"
#include "GenericMesh.h"
#include "ShapeContextUnit.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class SCRenderer {
private:
	static GLuint mProgramId;
	static GLuint mMeshProgramId;
	static GLuint mRayProgramId;
protected:
	void initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title, bool standAlone);
	static void setupGlutCallBacks();

public:

	static bool SCRenderer::mSetupDone;

	static GLuint SCRenderer::mVBO;
	static GLuint SCRenderer::mIBO;
	static GLuint SCRenderer::mVBOPoints;
	static GLuint SCRenderer::mVBOMesh;
	static GLuint SCRenderer::mVBORays;
	static int SCRenderer::mVBOSize;

	static int SCRenderer::vertexCount;
	static int SCRenderer::indexCount;
	static int SCRenderer::pointCount;
	static int SCRenderer::rayCount;
	static int SCRenderer::mainMeshvCount;
	static GLuint SCRenderer::uMVPMatrix;
	static GLuint SCRenderer::uMVPMatrix2;
	static float mTX, mTY,mTZ;

	static int windowX, windowY;
	static float FOV, nearPlane, farPlane;
	GLuint getRenderedTexture();
	static void renderScene();

	SCRenderer() {
		SCRenderer::mVBO = 0;
		SCRenderer::mVBOPoints = 0;
		SCRenderer::mVBOMesh = 0;
		SCRenderer::mVBORays = 0;
		GLuint mMeshProgramId = 0;
		GLuint mRayProgramId = 0;
		mProgramId = 0;
	
	};
	SCRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title, bool standAlone = true) {
		initRenderer(argc, argv, sizeX, sizeY, title, standAlone);
	}
	void updateCamera(float tVal[]) {
		mTX += tVal[0];
		mTY += tVal[1];
		mTZ += tVal[1];
	}
	void ConstructBinMesh();

	void setWindow(int windowX, int windowY) {
		this->windowX = windowX;
		this->windowY = windowY;
	}

	void setupRendererParameters(
		const char* vertexShader,
		const char* fragmentShader,
		const char* rayVertexShader,
		const char* rayFragmentShader,
		GenericMesh mesh,
		ShapeContextUnit units);

	~SCRenderer() {
		if (mProgramId > 0) {
			glDeleteProgram(mProgramId);
		}

		if (mMeshProgramId > 0) {
			glDeleteProgram(mMeshProgramId);
		}

		if (mRayProgramId > 0) {
			glDeleteProgram(mRayProgramId);
		}
		
		if (mVBO > 0)
			glDeleteBuffers(1, &SCRenderer::mVBO);

		if (mVBOPoints > 0)
			glDeleteBuffers(1, &SCRenderer::mVBOPoints);
		
		if (mVBOMesh > 0)
			glDeleteBuffers(1, &SCRenderer::mVBOMesh);
		
		if (mVBORays > 0)
			glDeleteBuffers(1, &SCRenderer::mVBORays);

	}
};

#endif