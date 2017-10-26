#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;

out vec4 vColor;

void main(){
	gl_Position = vec4(aPosition.x,aPosition.y,aPosition.z, 1.0);
	vColor = aColor;
}