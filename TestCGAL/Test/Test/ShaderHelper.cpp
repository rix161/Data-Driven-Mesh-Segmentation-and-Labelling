#include "ShaderHelper.h"
#include "FileReader.h"
#include <iostream>

GLuint ShaderProgramHelper::generateShader(GLenum shaderType, const char* shaderCode) {
	
	GLuint shaderId = -1;

	shaderId = glCreateShader(shaderType);
	
	const GLchar *shader[1];
	shader[0] = shaderCode;
	GLint length[1] = { (GLint)strlen(shaderCode) };

	glShaderSource(shaderId, 1, shader, length);
	glCompileShader(shaderId);

	GLint result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (!result) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shaderId, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling '%s': '%s'\n",shaderCode, InfoLog);
		return -1;
	}

	return shaderId;
}

int ShaderProgramHelper::generateProgram(const char* vertexShader, const char* fragmentShader) {
	
	FileReader fReader;
	GLint status;
	GLchar errorStatus[1024] = { 0 };

	std::string vertexShaderCode = fReader.getFile(vertexShader);
	std::cout<< vertexShaderCode;
	std::string fragmentShaderCode = fReader.getFile(fragmentShader);
	std::cout << vertexShaderCode;
	
	if (vertexShaderCode.empty() || fragmentShaderCode.empty())
		return -1;

	this->mVertexShaderId = generateShader(GL_VERTEX_SHADER,vertexShaderCode.c_str());
	if (this->mVertexShaderId == -1)
		return -1;

	this->mFragmentShaderId = generateShader(GL_FRAGMENT_SHADER, fragmentShaderCode.c_str());
	if (this->mFragmentShaderId == -1)
		return -1;

	this->mProgramId = glCreateProgram();
	if (mProgramId == 0) return -1;
	
	glAttachShader(mProgramId, mVertexShaderId);
	glAttachShader(mProgramId, mFragmentShaderId);
	
	glLinkProgram(mProgramId);
	glGetProgramiv(mProgramId, GL_LINK_STATUS, &status);
	if (status == 0) {
		glGetProgramInfoLog(mProgramId, sizeof(errorStatus), NULL,errorStatus);
		fprintf(stderr, "Error linking shader program: '%s'\n",errorStatus);
		return false;
	}

	glValidateProgram(mProgramId);
	glGetProgramiv(mProgramId, GL_VALIDATE_STATUS, &status);
	if (status == 0) {
		glGetProgramInfoLog(mProgramId, sizeof(errorStatus), NULL, errorStatus);
		fprintf(stderr, "Error Validating shader program: '%s'\n", errorStatus);
		return false;
	}
	
	return mProgramId;
}


ShaderProgramHelper::~ShaderProgramHelper() {
	
	if (mVertexShaderId != -1) {
		glDeleteShader(mVertexShaderId);
	}

	if (mFragmentShaderId != -1) {
		glDeleteShader(mFragmentShaderId);
	}
}