#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;

uniform float _Time;

void main(){
	UV = vUV;
	vec3 pos = vPos;
	pos *= 0.8; //scaling
	vec3 offset = vec3(abs(sin(_Time) * 0.5) - 0.1, abs(sin(_Time * 2.0)) * 0.5 - 0.2, 0); //movement
	gl_Position = vec4(pos + offset, 1.0);
}