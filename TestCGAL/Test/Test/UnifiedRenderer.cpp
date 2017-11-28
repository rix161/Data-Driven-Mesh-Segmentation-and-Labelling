#include "UnifiedRenderer.h"
#include "VIS.h"
#include "Curvature.h"

int UnifiedRenderer::windowX;
int UnifiedRenderer::windowY;
SDFRenderer UnifiedRenderer::mSDFRenderer;
AGDRenderer UnifiedRenderer::mAGDRenderer;
AGDRenderer UnifiedRenderer::mCurRenderer;
AGDRenderer UnifiedRenderer::mResultRenderer;
int UnifiedRenderer::mRendererType;

void UnifiedRenderer::setupRendererParameters() {
	const char* fileName = "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\TestModels\\king.off";
	const char* fileNameAGD = "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\output\\output_king_unit.off";

	OffWriter writter;
	GenericMesh cMesh = writter.ReadFile(fileNameAGD);
	mAGDRenderer.setWindow(this->windowX, this->windowY);

	mAGDRenderer.setupRendererParameters(
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDVertexShader.vs",
		"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDFragmentShader.fs",
		cMesh);
	
}

void UnifiedRenderer::keyboardCallback(unsigned char featureChar, int x, int y) {
	std::cout << "feature:" << featureChar << std::endl;
	static int count = featureChar - '0';
	int featureCount = 2;
	
	const char* fileName = "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\TestModels\\king.off";
	std::pair<std::vector<SDFUnit>, std::vector<SDFUnit>> rays;
	VIS vis(fileName);
	Curvature curMesh(fileName);
	curMesh.crunchMesh();
	curMesh.computeMeshCurvature();

	const char* fileNameAGD = "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\output\\output_king_unit.off";
	const char* fileNameRes1 = "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\PaperData\\ColoredMesh\\real\\201.off";
	const char* fileNameRes2 = "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\PaperData\\ColoredMesh\\predict\\201.off";

	OffWriter writter;
	GenericMesh cMesh = writter.ReadFile(fileNameAGD);
	GenericMesh rMesh1 = writter.ReadFile(fileNameRes1);
	GenericMesh rMesh2 = writter.ReadFile(fileNameRes2);

	switch (featureChar)
	{
	case '0':
			mAGDRenderer.setWindow(windowX, windowY);
			mAGDRenderer.setupRendererParameters(
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDVertexShader.vs",
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDFragmentShader.fs",
			cMesh);
			mRendererType = 0;
			break;
	case '1':
			vis.crunchMesh();
			rays = vis.computeFaceVIS(30, 30);
			mSDFRenderer.setWindow(windowX, windowY);
			mSDFRenderer.setupRendererParameters(
				"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFVertexShader.vs",
				"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFFragmentShader.fs",
				"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFRayVertextShader.vs",
				"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFRayFragmentShader.fs",
				vis,
				rays.first);
			mRendererType = 1;
			break;
	case '2':
		mCurRenderer.setWindow(windowX, windowY);
		mCurRenderer.setupRendererParameters(
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\CurvatureVertexShader.vs",
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\CurvatureFragmentShader.fs",
			curMesh);
		mRendererType = 2;
		break;
	case '3':
		vis.crunchMesh();
		rays = vis.computeFaceVIS(30, 30);
		mSDFRenderer.setWindow(windowX, windowY);
		mSDFRenderer.setupRendererParameters(
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFVertexShader.vs",
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFFragmentShader.fs",
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFRayVertextShader.vs",
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFRayFragmentShader.fs",
			vis,
			rays.second);
		mRendererType = 3;
		break;

	case '5':
		mResultRenderer.setWindow(windowX, windowY);
		mResultRenderer.setupRendererParameters(
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDVertexShader.vs",
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDFragmentShader.fs",
			rMesh1);
		mResultRenderer.setupSecondMesh(rMesh2);
		mRendererType = 5;

	default:
		break;
	}

}


void UnifiedRenderer::render() {
	setupGlutCallBacks();
	glutMainLoop();
}
void UnifiedRenderer::renderScene() {
	switch (mRendererType)
	{
	case 0:
		mAGDRenderer.renderScene();
		break;
	case 2:
		mCurRenderer.renderScene();
		break;
	case 1:
	case 3:
		mSDFRenderer.renderScene();
		break;
	case 5:
		mResultRenderer.renderSceneDouble();
	default:
		break;
	}
}



void UnifiedRenderer::initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title, bool standAlone = true) {
	UnifiedRenderer::windowX = sizeX;
	UnifiedRenderer::windowY = sizeY;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(sizeX, sizeY);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(title);

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cout << "Error: '%s'\n" << glewGetErrorString(res);
		return;
	}
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void UnifiedRenderer::setupGlutCallBacks() {
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(keyboardCallback);
}