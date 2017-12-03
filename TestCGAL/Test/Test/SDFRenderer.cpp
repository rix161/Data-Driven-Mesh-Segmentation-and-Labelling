#include "SDFRenderer.h"
#include "Transform.h"

bool SDFRenderer::mSetupDone = -1;
GLuint SDFRenderer::mVBO = -1;
GLuint SDFRenderer::mVBO2 = -1;
int SDFRenderer::mVBOSize = 0;
int SDFRenderer::vertexCount = 0;
int SDFRenderer::rayCount = 0;

float SDFRenderer::mTX = 0;
float SDFRenderer::mTY = 0;
float SDFRenderer::mTZ = 0;

GLuint SDFRenderer::uMVPMatrix;

int SDFRenderer::windowX;
int SDFRenderer::windowY;
float SDFRenderer::FOV = 60.0f;
float SDFRenderer::nearPlane = 0.0;
float SDFRenderer::farPlane = 1000.0;
GLuint SDFRenderer::mProgramId = -1;
GLuint SDFRenderer::mRayProgramId = -1;
GLuint SDFRenderer::uMVPMatrix2 = -1;


GLuint SDFRenderer::getRenderedTexture() {
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


void SDFRenderer::setupRendererParameters(
	const char* vertexShader,
	const char* fragmentShader,
	const char* rayVertexShader,
	const char* rayFragmentShader,
	GenericMesh mesh,
	std::vector<SDFUnit> rays) {

	ShaderProgramHelper sHelper;
	
	SDFRenderer::mProgramId = sHelper.generateProgram(vertexShader, fragmentShader);
	SDFRenderer::mRayProgramId = sHelper.generateProgram(rayVertexShader, rayFragmentShader);

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
		//std::vector<int> faceColorValues = faceColor[faceIter];
		int count = 0;
		for (std::deque<int>::iterator it2 = faceValues.begin(); it2 != faceValues.end(); it2++) {
			Kernel::Point_3 tempPoint = vertexPoints.at(*it2);
			Kernel::Point_3 BaryPoint = barycenters[count % 3];
			count++;
			
			meshData.push_back((float)tempPoint.x());
			meshData.push_back((float)tempPoint.y());
			meshData.push_back((float)tempPoint.z());

			meshData.push_back((float)BaryPoint.x());
			meshData.push_back((float)BaryPoint.y());
			meshData.push_back((float)BaryPoint.z());

			meshData.push_back(1.0);
			meshData.push_back(1.0);
			meshData.push_back(1.0);
			meshData.push_back(1.0);
		}
	}

	SDFRenderer::rayCount = rays.size();
	std::vector<GLfloat> rayData;
	for (SDFUnit ray : rays) {
		Kernel::Point_3 source = ray.getStartPoint();
		Kernel::Point_3 destination = ray.getEndPoint();
		glm::vec3 color = ray.getColor();
		
		rayData.push_back(source.x());
		rayData.push_back(source.y());
		rayData.push_back(source.z());
		rayData.push_back(color[0]);
		rayData.push_back(color[1]);
		rayData.push_back(color[2]);

		rayData.push_back(destination.x());
		rayData.push_back(destination.y());
		rayData.push_back(destination.z());
		rayData.push_back(color[0]);
		rayData.push_back(color[1]);
		rayData.push_back(color[2]);
	}
	
	glUseProgram(SDFRenderer::mProgramId);
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshData[0])*meshData.size(), &meshData[0], GL_STATIC_DRAW);

	SDFRenderer::uMVPMatrix = glGetUniformLocation(SDFRenderer::mProgramId, "uMVPMatrix");

	glUseProgram(SDFRenderer::mRayProgramId);
	glGenBuffers(1, &mVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rayData[0])*rayData.size(), &rayData[0], GL_STATIC_DRAW);

	SDFRenderer::uMVPMatrix2 = glGetUniformLocation(SDFRenderer::mRayProgramId, "uMVPMatrix");

	SDFRenderer::mSetupDone = true;
	SDFRenderer::mVBOSize = sizeof(meshData[0]);
}

void SDFRenderer::renderScene() {

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(false);

	Transform mTransformation;
	static float angle = 0.002f;
	angle -= 0.0007f;

	mTransformation.translate(glm::vec3(-0.25+mTX, -1.0, -3.0+mTY));
	mTransformation.scale(glm::vec3(2.0, 2.0, 2.0));
	mTransformation.rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
	mTransformation.projection(SDFRenderer::FOV, SDFRenderer::windowX, SDFRenderer::windowY, SDFRenderer::nearPlane, SDFRenderer::farPlane);

	glUseProgram(SDFRenderer::mProgramId);
	glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mTransformation.getMatrix()));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, SDFRenderer::mVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SDFRenderer::mVBOSize * 10, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, SDFRenderer::mVBOSize * 10, (const GLvoid*)(SDFRenderer::mVBOSize * 3));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, SDFRenderer::mVBOSize * 10, (const GLvoid*)(SDFRenderer::mVBOSize * (3 + 3)));
	glDrawArrays(GL_TRIANGLES, 0, (3 * SDFRenderer::vertexCount));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glUseProgram(SDFRenderer::mRayProgramId);
	glLineWidth(2.0f);
	glUniformMatrix4fv(uMVPMatrix2, 1, GL_FALSE, glm::value_ptr(mTransformation.getMatrix()));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, SDFRenderer::mVBO2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SDFRenderer::mVBOSize * 6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, SDFRenderer::mVBOSize * 6, (const GLvoid*)(SDFRenderer::mVBOSize * 3));
	glDrawArrays(GL_LINES, 0, (2 * SDFRenderer::rayCount));
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glutSwapBuffers();
}



void SDFRenderer::initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title,bool standAlone=true) {
	SDFRenderer::windowX = sizeX;
	SDFRenderer::windowY = sizeY;

	if (standAlone) {
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
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void SDFRenderer::setupGlutCallBacks() {
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
}