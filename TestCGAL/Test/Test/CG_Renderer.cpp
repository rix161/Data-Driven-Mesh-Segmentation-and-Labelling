#include "CGRenderer.h"
#include <glm/glm.hpp>


bool CGRenderer::mSetupDone = -1;
GLuint CGRenderer::mVBO = -1;
int CGRenderer::mVBOSize = 0;
int CGRenderer::vertexCount = 0;
GLuint CGRenderer::uMVPMatrix;

int CGRenderer::windowX;
int CGRenderer::windowY;
float CGRenderer::FOV = 60.0f;
float CGRenderer::nearPlane = 0.0;
float CGRenderer::farPlane = 1000.0;



void CGRenderer::initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(sizeX, sizeY);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(title);
}

void CGRenderer::setupRendererParameters(const char* vertexShader,
	const char* fragmentShader,
	GenericMesh mesh,
	float alpha) {

	ShaderProgramHelper sHelper;
	mProgramId = sHelper.generateProgram(vertexShader, fragmentShader);

	std::vector<Kernel::Point_3> vertexPoints = mesh.getVertexIndex();
	std::vector<std::vector<int>>vertexColors = mesh.getVertexColor();
	std::vector<std::deque<int>>faceIndex = mesh.getFaceIndex();
	std::vector<std::vector<int>>faceColor = mesh.getFaceColor();
	CGRenderer::vertexCount = mesh.getFaceCount();
	int faceCount = mesh.getFaceCount();
	int edgeCount = mesh.getEdgeCount();

	std::vector<GLfloat> meshData;
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
			meshData.push_back(faceColorValues[3]*alpha);
		}
	}
	glUseProgram(mProgramId);
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshData[0])*meshData.size(), &meshData[0], GL_STATIC_DRAW);

	CGRenderer::uMVPMatrix = glGetUniformLocation(mProgramId, "uMVPMatrix");
	CGRenderer::mSetupDone = true;
	CGRenderer::mVBOSize = sizeof(meshData[0]);
}
