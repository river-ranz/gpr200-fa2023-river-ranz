#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _WoodTexture;
uniform sampler2D _NoiseTexture;

void main(){
	FragColor = texture(_WoodTexture, UV);
}