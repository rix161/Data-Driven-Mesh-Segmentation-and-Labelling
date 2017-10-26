#ifndef  __SHADER_HELPER__
#define __SHADER_HELPER__
#include <GL/glew.h>

class ShaderProgramHelper {

private:
	GLuint mVertexShaderId, mFragmentShaderId;
	GLuint mProgramId;
	GLuint generateShader(GLenum shaderType, const char* shaderCode);

public:
	ShaderProgramHelper() {
		mVertexShaderId = -1;
		mFragmentShaderId = -1;
		mProgramId = -1;
	}
	int generateProgram(const char* vertexShaderCode, const char* fragmentShaderCode);
	int getProgramId() { return mProgramId; }
	~ShaderProgramHelper();

};

#endif // ! __SHADER_HELPER__
