#include "AGDRenderer.h"


bool AGDRenderer::mSetupDone = -1;
GLuint AGDRenderer::mVBO = -1;
int AGDRenderer::mVBOSize = 0;
int AGDRenderer::vertexCount = 0;

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
	AGDRenderer::vertexCount = mesh.getVertexCount();
	int faceCount = mesh.getFaceCount();
	int edgeCount = mesh.getEdgeCount();

	std::vector<GLfloat> meshData;
	float scale = 0.5;
	for (int faceIter = 0; faceIter < faceIndex.size(); faceIter++) {
		std::deque<int> faceValues = faceIndex[faceIter];
		std::vector<int> faceColorValues = faceColor[faceIter];
		for (std::deque<int>::iterator it2 = faceValues.begin(); it2 != faceValues.end(); it2++) {
			Kernel::Point_3 tempPoint = vertexPoints.at(*it2);
			
			meshData.push_back(tempPoint.x());
			meshData.push_back(tempPoint.y());
			meshData.push_back(tempPoint.z());

			meshData.push_back(faceColorValues[0] / 255.0);
			meshData.push_back(faceColorValues[1] / 255.0);
			meshData.push_back(faceColorValues[2] / 255.0);
			meshData.push_back(faceColorValues[3]);
		}
	}
	glUseProgram(mProgramId);
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshData[0])*meshData.size(), &meshData[0], GL_STATIC_DRAW);
	AGDRenderer::mSetupDone = true;
	AGDRenderer::mVBOSize = sizeof(meshData[0]);
}

void AGDRenderer::renderScene() {
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, AGDRenderer::mVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, AGDRenderer::mVBOSize * 7, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, AGDRenderer::mVBOSize * 7, (const GLvoid*)(AGDRenderer::mVBOSize*3));
	glDrawArrays(GL_TRIANGLES, 0, (3* AGDRenderer::vertexCount));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glutSwapBuffers();
}


void AGDRenderer::initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title) {
	glutInit(&argc, argv);
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
}