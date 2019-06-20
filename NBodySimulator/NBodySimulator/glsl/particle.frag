#version 430 core

uniform sampler2D image;
uniform vec3 baseColor;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

in vec3 test;

void main(){
	fragColor = vec4(baseColor, 1.0f);
	fragColor = texture(image, gl_PointCoord) * fragColor;

	// glow map.
	float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 0.3f)
		brightColor = vec4(fragColor.rgb, 1.0f);
}