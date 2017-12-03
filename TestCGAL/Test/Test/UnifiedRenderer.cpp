#include "UnifiedRenderer.h"
#include "VIS.h"
#include "Curvature.h"
#include "ShapeContext.h"

int UnifiedRenderer::windowX;
int UnifiedRenderer::windowY;
SDFRenderer UnifiedRenderer::mSDFRenderer;
AGDRenderer UnifiedRenderer::mAGDRenderer;
AGDRenderer UnifiedRenderer::mCurRenderer;
AGDRenderer UnifiedRenderer::mResultRenderer;
SCRenderer UnifiedRenderer::mSCRenderer;
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

enum {
	AGD = 1,
	SDF,
	CUR,
	VI,
	SC,
	CL,
	EX
};

void UnifiedRenderer::menuSelection(int idCommand)
{
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

	const char* meshName = "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\offFiles\\TestModels\\king.off";
	const char* destFile = "F:\\Masters\\SemI\\Computer Graphics\\Input Meshes\\PaperData\\labeledDb_features\\Test\\AGD\\king.txt";
	ShapeContext context = ShapeContext(meshName, destFile);
	ShapeContextUnit unit = context.compute();

	//Dirty dirty hack 
	switch (idCommand)
	{
	case AGD:
		UnifiedRenderer::mAGDRenderer.setWindow(windowX, windowY);
		mAGDRenderer.setupRendererParameters(
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDVertexShader.vs",
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDFragmentShader.fs",
			cMesh);
		mRendererType = 0;
		break;

	case SDF:
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

	case CUR:
		mCurRenderer.setWindow(windowX, windowY);
		mCurRenderer.setupRendererParameters(
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\CurvatureVertexShader.vs",
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\CurvatureFragmentShader.fs",
			curMesh);
		mRendererType = 2;
		break;
		
	case VI:
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

	case SC:
		mSCRenderer.setWindow(windowX, windowY);
		mSCRenderer.setupRendererParameters(
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFVertexShader.vs",
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFFragmentShader.fs",
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SDFRayVertextShader.vs",
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\SCFragmentShader.fs",
			cMesh,
			unit);
		mRendererType = 4;
		break;

	case CL:
		mResultRenderer.setWindow(windowX, windowY);
		mResultRenderer.setupRendererParameters(
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDVertexShader.vs",
			"F:\\Masters\\SemI\\Computer Graphics\\TestCGAL\\Test\\Test\\AGDFragmentShader.fs",
			rMesh1);
		mResultRenderer.setupSecondMesh(rMesh2);
		mRendererType = 5;
		break;

	case EX:
		exit(0);
		break;
	
	}
	
	glutPostRedisplay();
}


int UnifiedRenderer::buildMenu(void)
{
	int menu;
	menu = glutCreateMenu(menuSelection);
	glutAddMenuEntry("Average Geodesic Distance", AGD);
	glutAddMenuEntry("Shape Diameter Function", SDF);
	glutAddMenuEntry("Curvature", CUR);
	glutAddMenuEntry("Volumetric Shape Images", VI);
	glutAddMenuEntry("Shape Context", SC);
	glutAddMenuEntry("Classification", CL);
	glutAddMenuEntry("Exit demo", EX);
	return menu;
}

void UnifiedRenderer::updateRenderer(float val[]) {

	switch (mRendererType)
	{
	case 0:
		mAGDRenderer.updateCamera(val);
		break;
	case 2:
		mCurRenderer.updateCamera(val);
		break;
	case 1:
	case 3:
		mSDFRenderer.updateCamera(val);
		break;
	case 4:
		mSCRenderer.updateCamera(val);
		break;
	case 5:
		mResultRenderer.updateCamera(val);
	default:
		break;
	}

}


void UnifiedRenderer::keyboardCallback(unsigned char featureChar, int x, int y) {
	float tVal[3];
	std::cout << "Char:" << featureChar << std::endl;
	switch (featureChar) {
		case 'w':
			tVal[0] = 0.0f;
			tVal[1] = 0.05f;
			tVal[2] = 0.0f;
			updateRenderer(tVal);
			break;
		case 's':
			tVal[0] = 0.0;
			tVal[1] = -0.05;
			tVal[2] = 0.0;
			updateRenderer(tVal);
			break;
		case 'a':
			tVal[0] = -0.05;
			tVal[1] = 0.0;
			tVal[2] = 0.0;
			updateRenderer(tVal);
			break;
		case 'd':
			tVal[0] = 0.05;
			tVal[1] = 0.0;
			tVal[2] = 0.0;
			updateRenderer(tVal);
			break;
	}
	glutPostRedisplay();
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
	case 4:
		mSCRenderer.renderScene();
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
	this ->buildMenu();
	glutAttachMenu(GLUT_RIGHT_BUTTON);

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