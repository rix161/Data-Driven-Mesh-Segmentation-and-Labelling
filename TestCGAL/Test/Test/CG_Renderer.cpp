#include "CGRenderer.h"
#include <glm/glm.hpp>

void CGRenderer::initRenderer(int *argc, char** argv, int sizeX, int sizeY, const char* title) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(sizeX, sizeY);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(title);

	setupGlutCallBacks();

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cout<<"Error: '%s'\n"<<glewGetErrorString(res);
		return;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void CGRenderer::renderScene() {}

void CGRenderer::setupRendererParameters(std::vector<Kernel::Point_3> vertexPoints,
	std::vector<std::vector<int>>vertexColors,
	std::vector<std::deque<int>>faceIndex,
	std::vector<std::vector<int>>faceColor,
	int vertexCount, int faceCount, int edgeCount) {}

void CGRenderer::setupGlutCallBacks() {
	glutDisplayFunc(renderScene);
}
