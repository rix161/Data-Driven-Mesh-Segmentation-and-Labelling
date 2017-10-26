#include "AGDRenderer.h"


bool AGDRenderer::mSetupDone = -1;
GLuint AGDRenderer::mVBO = -1;

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
	int vertexCount = mesh.getVertexCount();
	int faceCount = mesh.getFaceCount();
	int edgeCount = mesh.getEdgeCount();

	std::vector<GLfloat> meshData;

	for (std::vector<std::deque<int>>::iterator it = faceIndex.begin(); it != faceIndex.end(); it++) {
		for (std::deque<int>::iterator it2 = (*it).begin(); it2 != (*it).end(); it2++) {

			Kernel::Point_3 tempPoint = vertexPoints.at(*it2);
			std::vector<int> tempColor = faceColor.at(*it2);
			
			meshData.push_back(tempPoint.x());
			meshData.push_back(tempPoint.y());
			meshData.push_back(tempPoint.z());

			meshData.push_back(tempColor[0]);
			meshData.push_back(tempColor[1]);
			meshData.push_back(tempColor[2]);
			meshData.push_back(tempColor[3]);
		}
	}
	
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshData[0])*meshData.size(), &meshData[0], GL_STATIC_DRAW);
	AGDRenderer::mSetupDone = true;
}

void AGDRenderer::renderScene() {
	if (!AGDRenderer::mSetupDone) return;

	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, AGDRenderer::mVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);

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