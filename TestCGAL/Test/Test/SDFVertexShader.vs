#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aBaryCenter;
layout (location = 2) in vec4 aColor;
uniform mat4  uMVPMatrix;

out vec4 vColor;
out vec3 vBaryCenter;

void main(){
	gl_Position = uMVPMatrix * vec4(aPosition.x,aPosition.y,aPosition.z, 1.0);
	vColor = aColor;
	vBaryCenter = aBaryCenter;
}