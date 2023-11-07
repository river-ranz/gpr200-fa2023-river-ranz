#version 450
out vec4 FragColor;

in vec3 Normal;
in vec3 worldPos;

uniform vec3 _Color;

void main(){
	vec3 normal = normalize(Normal);
	FragColor = vec4(_Color, 1.0);
}