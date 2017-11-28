#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in float aColorScale;
uniform mat4  uMVPMatrix;

out vec4 vColor;

void main(){
	gl_Position = uMVPMatrix * vec4(aPosition.x,aPosition.y,aPosition.z, 1.0);
	vColor =  vec4(aPosition.x,aPosition.y,aPosition.z, 1.0)*aColorScale+0.05;
}