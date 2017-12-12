#include "SCRenderer.h"
#include "Transform.h"

bool SCRenderer::mSetupDone = -1;
GLuint SCRenderer::mVBO = -1;
GLuint SCRenderer::mIBO = -1;
GLuint SCRenderer::mVBOPoints = -1;
GLuint SCRenderer::mVBOMesh = -1;
GLuint SCRenderer::mVBORays = -1;
int SCRenderer::mVBOSize = 0;
int SCRenderer::vertexCount = 0;
int SCRenderer::indexCount = 0;
int SCRenderer::pointCount = 0;
int SCRenderer::rayCount = 0;
float SCRenderer::mTX = 0;
float SCRenderer::mTY = 0;
float SCRenderer::mTZ = 0;
GLuint SCRenderer::uMVPMatrix;

int SCRenderer::windowX;
int SCRenderer::windowY;
float SCRenderer::FOV = 60.0f;
float SCRenderer::nearPlane = 0.0;
float SCRenderer::farPlane = 1000.0;
GLuint SCRenderer::mProgramId = -1;
GLuint SCRenderer::uMVPMatrix2 = -1;
int SCRenderer::mainMeshvCount = 0;
GLuint SCRenderer::mMeshProgramId = 0;
GLuint SCRenderer::mRayProgramId = 0;

//Taken and heavily modified from stackoverFlow post.
class SolidSphere
{
protected:
	std::vector<GLfloat> vertices;
	std::vector<GLushort> indices;


public:
	SolidSphere(float radius, unsigned int stacks, unsigned int slices)
	{
		double pi = 3.14;
		std::vector<GLushort>::iterator i = indices.begin();

		for (int stackNumber = 0; stackNumber <= stacks; ++stackNumber)
		{
			std::cout << "***************************************" << std::endl;
			for (int sliceNumber = 0; sliceNumber < slices; ++sliceNumber)
			{
				float theta = (float)(stackNumber * pi / stacks);
				float phi = (float)(sliceNumber * 2 * pi / slices);
				float sinTheta = sin(theta);
				float sinPhi = sin(phi);
				float cosTheta = cos(theta);
				float cosPhi = cos(phi);

				vertices.push_back(radius*cosPhi * sinTheta);
				vertices.push_back(radius *cosTheta);
				vertices.push_back(radius*sinPhi * sinTheta);
				
				vertices.push_back(1.0);
				vertices.push_back(1.0);
				vertices.push_back(1.0);
				vertices.push_back(1.0);


				int v1 = stackNumber*slices + sliceNumber;
				int v2 = (stackNumber + 1)*slices + sliceNumber;
				int v3 = (stackNumber + 1)*slices + (sliceNumber + 1);
				int v4 = stackNumber*slices + (sliceNumber + 1);
				if (sliceNumber == slices - 1) {
					if (stackNumber != stacks-1) {
						v3 = v3 - slices;
						v4 = v4 - slices;
					}
				}

				std::cout << "StackNum" << stackNumber << " SliceNo:" << sliceNumber << " v1:" << v1 << " v2:" << v2 << " v3:" << v3 << "v4:" << v4 << std::endl;
				indices.push_back(v1);
				indices.push_back(v2);

				indices.push_back(v2);
				indices.push_back(v3);
				
				indices.push_back(v3);
				indices.push_back(v4);
				
				indices.push_back(v4);
				indices.push_back(v1);

			}
			std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
		}
		std::cout << "FIN" << std::endl;

	}

	std::vector<GLfloat> getVertices() {
		return vertices;
	}

	std::vector<GLushort> getIndices() {
		return indices;
	}
};

void SCRenderer::ConstructBinMesh() {
	SolidSphere sphere(2, 12, 24);

}

void SCRenderer::setupRendererParameters(
	const char* vertexShader,
	const char* fragmentShader,
	const char* histoVertexShader,
	const char* histoFragmentShader,
	GenericMesh mesh,
	ShapeContextUnit scUnits) {

	ShaderProgramHelper sHelper;
	SCRenderer::mProgramId = sHelper.generateProgram(histoVertexShader, histoFragmentShader);
	SCRenderer::mMeshProgramId = sHelper.generateProgram(vertexShader, fragmentShader);

	SolidSphere sphere(1.1, 5, 6);
	std::vector<GLfloat> meshData = sphere.getVertices();
	std::vector<GLushort> indexData = sphere.getIndices();
	glUseProgram(SCRenderer::mProgramId);
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshData[0])*meshData.size(), &meshData[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData[0])*indexData.size(), &indexData[0], GL_STATIC_DRAW);

	SCRenderer::uMVPMatrix = glGetUniformLocation(SCRenderer::mProgramId, "uMVPMatrix");

	std::vector<GLfloat> pointData;
	std::vector<Kernel::Vector_3> vecs = scUnits.getSCPoints();
	
	for (Kernel::Vector_3 vec : vecs) {
		if (vec.squared_length() > 0.0) {
			pointData.push_back(vec[0]);
			pointData.push_back(vec[1]);
			pointData.push_back(vec[2]);

			pointData.push_back(1.0);
			pointData.push_back(1.0);
			pointData.push_back(0.0);
			pointData.push_back(1.0);
		}
	}

	glGenBuffers(1, &mVBOPoints);
	glBindBuffer(GL_ARRAY_BUFFER, mVBOPoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pointData[0])*pointData.size(), &pointData[0], GL_STATIC_DRAW);

	SCRenderer::rayCount = scUnits.getAllFaceVector().size();
	std::vector<GLfloat> rayData;
	Kernel::Point_3 source = scUnits.getFacePoint();
	std::vector<Kernel::Point_3> dests = scUnits.getAllFaceVector();
	float color[] = { 0.0,0.0,1.0 };
	for (Kernel::Point_3 facePoint : dests) {
		Kernel::Point_3 destination = facePoint;
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

	glGenBuffers(1, &mVBORays);
	glBindBuffer(GL_ARRAY_BUFFER, mVBORays);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rayData[0])*rayData.size(), &rayData[0], GL_STATIC_DRAW);

	std::vector<Kernel::Point_3> vertexPoints = mesh.getVertexIndex();
	std::vector<std::vector<int>>vertexColors = mesh.getVertexColor();
	std::vector<std::deque<int>>faceIndex = mesh.getFaceIndex();
	std::vector<std::vector<int>>faceColor = mesh.getFaceColor();
	std::vector <Kernel::Point_3>barycenters;

	barycenters.push_back(Kernel::Point_3(1.0, 0.0, 0.0));
	barycenters.push_back(Kernel::Point_3(0.0, 1.0, 0.0));
	barycenters.push_back(Kernel::Point_3(0.0, 0.0, 1.0));

	SCRenderer::mainMeshvCount = mesh.getFaceCount();
	int faceCount = mesh.getFaceCount();
	int edgeCount = mesh.getEdgeCount();

	std::vector<GLfloat> mainMeshData;
	for (int faceIter = 0; faceIter < faceIndex.size(); faceIter++) {
		std::deque<int> faceValues = faceIndex[faceIter];
		//std::vector<int> faceColorValues = faceColor[faceIter];
		int count = 0;
		for (std::deque<int>::iterator it2 = faceValues.begin(); it2 != faceValues.end(); it2++) {
			Kernel::Point_3 tempPoint = vertexPoints.at(*it2);
			Kernel::Point_3 BaryPoint = barycenters[count % 3];
			count++;

			mainMeshData.push_back(tempPoint.x());
			mainMeshData.push_back(tempPoint.y());
			mainMeshData.push_back(tempPoint.z());

			mainMeshData.push_back(BaryPoint.x());
			mainMeshData.push_back(BaryPoint.y());
			mainMeshData.push_back(BaryPoint.z());

			mainMeshData.push_back(1.0);
			mainMeshData.push_back(1.0);
			mainMeshData.push_back(1.0);
			mainMeshData.push_back(1.0);
		}
	}

	glUseProgram(SCRenderer::mMeshProgramId);
	SCRenderer::uMVPMatrix2 = glGetUniformLocation(SCRenderer::mMeshProgramId, "uMVPMatrix");
	glGenBuffers(1, &mVBOMesh);
	glBindBuffer(GL_ARRAY_BUFFER, mVBOMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mainMeshData[0])*mainMeshData.size(), &mainMeshData[0], GL_STATIC_DRAW);

	SCRenderer::mSetupDone = true;
	SCRenderer::mVBOSize = sizeof(meshData[0]);
	SCRenderer::vertexCount = mainMeshData.size();
	SCRenderer::indexCount = indexData.size();
	SCRenderer::pointCount = pointData.size();
}

void SCRenderer::renderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Transform mTransformation, mTransformation2;
	static float angle = 0.002;
	angle -= 0.0007;

	mTransformation.translate(glm::vec3(-0.75+mTX, -0.5+mTY, -2.5+mTZ));
	mTransformation.rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
	mTransformation.projection(SCRenderer::FOV, SCRenderer::windowX, SCRenderer::windowY, SCRenderer::nearPlane, SCRenderer::farPlane);
	
	//Set i = 5 for a trippy ride!
	for (int i = 1; i <= 1; i++) {
		mTransformation.scale(glm::vec3(.5, 0.5, 0.5));

		glUseProgram(SCRenderer::mProgramId);
		glLineWidth(2.0f);
		glPointSize(5.0f);
		glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mTransformation.getMatrix()));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, SCRenderer::mVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SCRenderer::mVBOSize * 7, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, SCRenderer::mVBOSize * 7, (const GLvoid*)(SCRenderer::mVBOSize * 3));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SCRenderer::mIBO);
		glDrawElements(GL_LINES, (indexCount - 1) * sizeof(GL_UNSIGNED_SHORT), GL_UNSIGNED_SHORT, (void*)0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, SCRenderer::mVBOPoints);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SCRenderer::mVBOSize * 7, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, SCRenderer::mVBOSize * 7, (const GLvoid*)(SCRenderer::mVBOSize * 3));
		glDrawArrays(GL_POINTS, 0, SCRenderer::pointCount);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	mTransformation2.translate(glm::vec3(0.75+mTX, -1.5 + mTY, -5.0+mTZ));
	mTransformation2.rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
	mTransformation2.projection(SCRenderer::FOV, SCRenderer::windowX, SCRenderer::windowY, SCRenderer::nearPlane, SCRenderer::farPlane);
	mTransformation2.scale(glm::vec3(3.0, 3.0, 3.0));

	glUniformMatrix4fv(uMVPMatrix2, 1, GL_FALSE, glm::value_ptr(mTransformation2.getMatrix()));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, SCRenderer::mVBORays);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SCRenderer::mVBOSize * 6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, SCRenderer::mVBOSize * 6, (const GLvoid*)(SCRenderer::mVBOSize * 3));
	glDrawArrays(GL_LINES, 0, (2 * SCRenderer::rayCount));
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glUseProgram(SCRenderer::mMeshProgramId);
	glUniformMatrix4fv(uMVPMatrix2, 1, GL_FALSE, glm::value_ptr(mTransformation2.getMatrix()));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, SCRenderer::mVBOMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SCRenderer::mVBOSize * 10, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, SCRenderer::mVBOSize * 10, (const GLvoid*)(SCRenderer::mVBOSize * 3));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, SCRenderer::mVBOSize * 10, (const GLvoid*)(SCRenderer::mVBOSize * (3 + 3)));
	glDrawArrays(GL_TRIANGLES, 0, (3 * SCRenderer::vertexCount));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glutSwapBuffers();
}



void SCRenderer::initRenderer(int argc, char** argv, int sizeX, int sizeY, const char* title, bool standAlone = true) {
	SCRenderer::windowX = sizeX;
	SCRenderer::windowY = sizeY;

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

void SCRenderer::setupGlutCallBacks() {
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
}




GLuint SCRenderer::getRenderedTexture() {
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