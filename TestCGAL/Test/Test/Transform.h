#ifndef  __TRANSFORM__
#define __TRANSFORM__
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>


class Transform {
private:
	glm::mat4 mModelMat;
	glm::mat4 mViewMat;
	glm::mat4 mProjectionMat;

public:
	Transform() {
		mModelMat = glm::mat4(1.0);
		mViewMat = glm::mat4(1.0);
		mProjectionMat = glm::mat4(1.0);

	}

	void rotate(float angle, glm::vec3 axis) {
		mModelMat = glm::rotate(mModelMat, angle, axis);
	}

	void translate(glm::vec3 translateVec) {
		mModelMat = glm::translate(mModelMat, translateVec);
	}

	void scale(glm::vec3 scaleXYZ) {
		mModelMat = glm::scale(mModelMat, scaleXYZ);
	}
	void projection(float fov, int windowX, int windowY, float nearPlane, float farPlane) {
		mProjectionMat = glm::perspective(glm::radians(fov), (float)(windowX / windowY), nearPlane, farPlane);
	}

	void printMatix() {
		std::cout << glm::to_string(mModelMat) << std::endl;
	}

	glm::mat4 getMatrix() { return mProjectionMat*mModelMat; }
};

#endif // ! __TRANSFORM__
