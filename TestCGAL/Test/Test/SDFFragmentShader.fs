#version 330  

in vec4 vColor;
in vec3 vBaryCenter;
out vec4 FragColor;

void main(){
	vec4 finalColor;
	if(any(lessThan(vBaryCenter,vec3(0.009)))){
		finalColor = vec4(1.0,1.0,1.0,1.0);
	}else{
		finalColor = vec4(0.0,0.0,0.0,0.0);
	}
	FragColor = finalColor;
}
