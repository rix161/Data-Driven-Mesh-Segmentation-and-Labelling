#include "SDFRenderer.h"
#include "Transform.h"

bool SDFRenderer::mSetupDone = -1;
GLuint SDFRenderer::mVBO = -1;
int SDFRenderer::mVBOSize = 0;
int SDFRenderer::vertexCount = 0;
GLuint SDFRenderer::uMVPMatrix;

int SDFRenderer::windowX;
int SDFRenderer::windowY;
float SDFRenderer::FOV = 60.0f;
float SDFRenderer::nearPlane = 0.0;
float SDFRenderer::farPlane = 1000.0;

void SDFRenderer::setupRendererParameters(
	const char* vertexShader,
	const char* fragmentShader,
	GenericMesh mesh,
	float alpha) {
	ShaderProgramHelper sHelper;
	mProgramId = sHelper.generateProgram(vertexShader, fragmentShader);

	std::vector<Kernel::Point_3> vertexPoints = mesh.getVertexIndex();
	std::vector<std::vector<int>>vertexColors = mesh.getVertexColor();
	std::vector<std::deque<int>>faceIndex = mesh.getFaceIndex();
	std::vector<std::vector<int>>faceColor = mesh.getFaceColor();
	std::vector <Kernel::Point_3>barycenters;

	barycenters.push_back(Kernel::Point_3(1.0, 0.0, 0.0));
	barycenters.push_back(Kernel::Point_3(0.0, 1.0, 0.0));
	barycenters.push_back(Kernel::Point_3(0.0, 0.0, 1.0));


	SDFRenderer::vertexCount = mesh.getFaceCount();
	int faceCount = mesh.getFaceCount();
	int edgeCount = mesh.getEdgeCount();

	std::vector<GLfloat> meshData;
	for (int faceIter = 0; faceIter < faceIndex.size(); faceIter++) {
		std::deque<int> faceValues = faceIndex[faceIter];
		std::vector<int> faceColorValues = faceColor[faceIter];
		
		int count = 0;
		for (std::deque<int>::iterator it2 = faceValues.begin(); it2 != faceValues.end(); it2++) {
			Kernel::Point_3 tempPoint = vertexPoints.at(*it2);
			Kernel::Point_3 BaryPoint = barycenters[count % 3];
			count++;
			
			meshData.push_back(tempPoint.x());
			meshData.push_back(tempPoint.y());
			meshData.push_back(tempPoint.z());

			meshData.push_back(BaryPoint.x());
			meshData.push_back(BaryPoint.y());
			meshData.push_back(BaryPoint.z());

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

	SDFRenderer::uMVPMatrix = glGetUniformLocation(mProgramId, "uMVPMatrix");
	SDFRenderer::mSetupDone = true;
	SDFRenderer::mVBOSize = sizeof(meshData[0]);
}

void SDFRenderer::renderScene() {

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(false);

	Transform mTransformation;
	static float angle = 0.002;
	angle += 0.0002;

	mTransformation.translate(glm::vec3(-0.5, -0.5, -3.0));
	mTransformation.rotate(angle, glm::vec3(1.0f, 1.0f, 0.0f));
	mTransformation.scale(glm::vec3(1.0, 1.0, 1.0));

	mTransformation.projection(SDFRenderer::FOV, SDFRenderer::windowX, SDFRenderer::windowY, SDFRenderer::nearPlane, SDFRenderer::farPlane);

	glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mTransformation.getMatrix()));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, SDFRenderer::mVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SDFRenderer::mVBOSize * 10, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, SDFRenderer::mVBOSize * 10, (const GLvoid*)(SDFRenderer::mVBOSize * 3));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, SDFRenderer::mVBOSize * 10, (const GLvoid*)(SDFRenderer::mVBOSize * (3+3)));
	glDrawArrays(GL_TRIANGLES, 0, (3 * SDFRenderer::vertexCount));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glutSwapBuffers();
}



void SDFRenderer::initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title) {
	glutInit(&argc, argv);

	SDFRenderer::windowX = sizeX;
	SDFRenderer::windowY = sizeY;

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

void SDFRenderer::setupGlutCallBacks() {
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
}