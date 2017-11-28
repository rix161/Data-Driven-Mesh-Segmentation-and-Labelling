#include "AGDRenderer.h"
#include "Transform.h"

bool AGDRenderer::mSetupDone = -1;
GLuint AGDRenderer::mVBO = -1;
GLuint AGDRenderer::mVBO2 = -1;
int AGDRenderer::mVBOSize = 0;
int AGDRenderer::vertexCount = 0;
GLuint AGDRenderer::uMVPMatrix;

int AGDRenderer::windowX;
int AGDRenderer::windowY;
float AGDRenderer::FOV=60.0f;
float AGDRenderer::nearPlane=0.0;
float AGDRenderer::farPlane=1000.0;

void AGDRenderer::setupRendererParameters(
	const char* vertexShader,
	const char* fragmentShader,
	GenericMesh mesh) {
	ShaderProgramHelper sHelper;
	mProgramId = sHelper.generateProgram(vertexShader, fragmentShader);

	std::vector<Kernel::Point_3> vertexPoints = mesh.getVertexIndex();
	std::vector<std::vector<int>>vertexColors = mesh.getVertexColor();
	std::vector<std::deque<int>>faceIndex = mesh.getFaceIndex();
	std::vector<std::vector<int>>faceColor = mesh.getFaceColor();
	std::vector<float>faceColorScale = mesh.getColorScales();
	AGDRenderer::vertexCount = mesh.getFaceCount();
	int faceCount = mesh.getFaceCount();
	int edgeCount = mesh.getEdgeCount();
	bool doColorScale = mesh.isInterpolate();

	std::vector<GLfloat> meshData;
	int i = 0;
	for (int faceIter = 0; faceIter < faceIndex.size()-1; faceIter++) {
		std::deque<int> faceValues = faceIndex[faceIter];
		std::vector<int> faceColorValues = faceColor[faceIter];
		float colorScale = 1.0;
		if (doColorScale) colorScale = faceColorScale[faceIter];
		
		for (std::deque<int>::iterator it2 = faceValues.begin(); it2 != faceValues.end(); it2++) {
			Kernel::Point_3 tempPoint = vertexPoints.at(*it2);
			
			meshData.push_back(tempPoint.x());
			meshData.push_back(tempPoint.y());
			meshData.push_back(tempPoint.z());

			meshData.push_back(faceColorValues[0] / 255.0);
			meshData.push_back(faceColorValues[1] / 255.0);
			meshData.push_back(faceColorValues[2] / 255.0);
			meshData.push_back(faceColorValues[3]);
			meshData.push_back(colorScale);
		}
	}
	glUseProgram(mProgramId);
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshData[0])*meshData.size(), &meshData[0], GL_STATIC_DRAW);
	
	AGDRenderer::uMVPMatrix = glGetUniformLocation(mProgramId, "uMVPMatrix");
	AGDRenderer::mSetupDone = true;
	AGDRenderer::mVBOSize = sizeof(meshData[0]);
}


void AGDRenderer::setupSecondMesh(GenericMesh mesh){

	std::vector<Kernel::Point_3> vertexPoints = mesh.getVertexIndex();
	std::vector<std::vector<int>>vertexColors = mesh.getVertexColor();
	std::vector<std::deque<int>>faceIndex = mesh.getFaceIndex();
	std::vector<std::vector<int>>faceColor = mesh.getFaceColor();
	std::vector<float>faceColorScale = mesh.getColorScales();
	AGDRenderer::vertexCount = mesh.getFaceCount();
	int faceCount = mesh.getFaceCount();
	int edgeCount = mesh.getEdgeCount();
	bool doColorScale = mesh.isInterpolate();

	std::vector<GLfloat> meshData;
	int i = 0;
	for (int faceIter = 0; faceIter < faceIndex.size() - 1; faceIter++) {
		std::deque<int> faceValues = faceIndex[faceIter];
		std::vector<int> faceColorValues = faceColor[faceIter];
		float colorScale = 1.0;
		if (doColorScale) colorScale = faceColorScale[faceIter];

		for (std::deque<int>::iterator it2 = faceValues.begin(); it2 != faceValues.end(); it2++) {
			Kernel::Point_3 tempPoint = vertexPoints.at(*it2);

			meshData.push_back(tempPoint.x());
			meshData.push_back(tempPoint.y());
			meshData.push_back(tempPoint.z());

			meshData.push_back(faceColorValues[0] / 255.0);
			meshData.push_back(faceColorValues[1] / 255.0);
			meshData.push_back(faceColorValues[2] / 255.0);
			meshData.push_back(faceColorValues[3]);
			meshData.push_back(colorScale);
		}
	}

	glGenBuffers(1, &mVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshData[0])*meshData.size(), &meshData[0], GL_STATIC_DRAW);

}

GLuint AGDRenderer::getRenderedTexture() {
	GLuint renderedFB = 0;
	glGenFramebuffers(1, &renderedFB);
	glBindFramebuffer(GL_FRAMEBUFFER, renderedFB);

	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowX, windowY, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Unable to create FrameBuffer" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, renderedFB);
	renderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return renderedTexture;
}

void AGDRenderer::renderSceneDouble() {

	int strideCount = 8;
	int vertexCount = 3;
	int colorCount = 4;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Transform mTransformation, mTransformation2;
	static float angle = 0.002;
	angle -= 0.0007;

	mTransformation.translate(glm::vec3(-0.75, -0.5, -3.0));
	mTransformation.scale(glm::vec3(1.0, 1.0, 1.0));
	mTransformation.rotate(angle, glm::vec3(1.f, 0.0f, 0.0f));
	mTransformation.projection(AGDRenderer::FOV, AGDRenderer::windowX, AGDRenderer::windowY, AGDRenderer::nearPlane, AGDRenderer::farPlane);
	glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mTransformation.getMatrix()));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, AGDRenderer::mVBO);
	glVertexAttribPointer(0, vertexCount, GL_FLOAT, GL_FALSE, AGDRenderer::mVBOSize * strideCount, 0);
	glVertexAttribPointer(1, colorCount, GL_FLOAT, GL_FALSE, AGDRenderer::mVBOSize * strideCount, (const GLvoid*)(AGDRenderer::mVBOSize * 3));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, AGDRenderer::mVBOSize * strideCount, (const GLvoid*)(AGDRenderer::mVBOSize * (vertexCount + colorCount)));
	glDrawArrays(GL_TRIANGLES, 0, (3 * AGDRenderer::vertexCount));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	mTransformation2.translate(glm::vec3(0.75, -0.5, -3.0));
	mTransformation2.scale(glm::vec3(1.0, 1.0, 1.0));
	mTransformation2.rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f));
	mTransformation2.projection(AGDRenderer::FOV, AGDRenderer::windowX, AGDRenderer::windowY, AGDRenderer::nearPlane, AGDRenderer::farPlane);
	glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mTransformation2.getMatrix()));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, AGDRenderer::mVBO2);
	glVertexAttribPointer(0, vertexCount, GL_FLOAT, GL_FALSE, AGDRenderer::mVBOSize * strideCount, 0);
	glVertexAttribPointer(1, colorCount, GL_FLOAT, GL_FALSE, AGDRenderer::mVBOSize * strideCount, (const GLvoid*)(AGDRenderer::mVBOSize * 3));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, AGDRenderer::mVBOSize * strideCount, (const GLvoid*)(AGDRenderer::mVBOSize * (vertexCount + colorCount)));
	glDrawArrays(GL_TRIANGLES, 0, (3 * AGDRenderer::vertexCount));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glutSwapBuffers();
}

void AGDRenderer::renderScene() {
	int strideCount = 8;
	int vertexCount = 3;
	int colorCount = 4;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Transform mTransformation;
	static float angle = 0.002;
	angle -= 0.0007;

	mTransformation.translate(glm::vec3(-0.5, -1.5, -3.0));
	mTransformation.scale(glm::vec3(1.5, 1.5, 1.5));
	mTransformation.rotate(angle, glm::vec3(0.25f, 1.0f, 0.0f));
	mTransformation.projection(AGDRenderer::FOV, AGDRenderer::windowX, AGDRenderer::windowY, AGDRenderer::nearPlane, AGDRenderer::farPlane);
	glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mTransformation.getMatrix()));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, AGDRenderer::mVBO);
	glVertexAttribPointer(0, vertexCount, GL_FLOAT, GL_FALSE, AGDRenderer::mVBOSize * strideCount, 0);
	glVertexAttribPointer(1, colorCount, GL_FLOAT, GL_FALSE, AGDRenderer::mVBOSize * strideCount, (const GLvoid*)(AGDRenderer::mVBOSize*3));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, AGDRenderer::mVBOSize * strideCount, (const GLvoid*)(AGDRenderer::mVBOSize * (vertexCount+colorCount)));
	glDrawArrays(GL_TRIANGLES, 0, (3* AGDRenderer::vertexCount));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glutSwapBuffers();
}



void AGDRenderer::initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title) {
	glutInit(&argc, argv);

	AGDRenderer::windowX = sizeX;
	AGDRenderer::windowY = sizeY;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(sizeX, sizeY);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(title);

	setupGlutCallBacks();

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cout << "Error: '%s'\n" << glewGetErrorString(res);
		return;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void AGDRenderer::setupGlutCallBacks() {
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
}