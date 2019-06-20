#version 430 core
layout (location = 0) in vec4 position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;
uniform float pointSize;

//out vec3 test;

void main(){
	vec3 eyeSpacePos = (viewMatrix * vec4(position.xyz, 1.0f)).xyz;
	gl_PointSize = max(1.0f, 800.0f * pointSize / (1.0f - eyeSpacePos.z));
	gl_Position = projectMatrix * viewMatrix * vec4(position.xyz, 1.0f);
	/*if(isnan(position.x) || isnan(position.y) || isnan(position.z))
	{
		test = vec3(1.0f, 0.0f, 0.0f);
		gl_Position = projectMatrix * viewMatrix * vec4(0.0, 0.0, 0.0, 1.0f);
		gl_PointSize = 500.0f;
	}
	else
		test = vec3(0.0f,1.0f,0.0f);*/
}