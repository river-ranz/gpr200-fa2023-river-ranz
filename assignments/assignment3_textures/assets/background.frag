#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _WoodTexture;
uniform sampler2D _ScarySmile;
uniform sampler2D _OverlayTexture;

void main(){
	float noise = texture(_OverlayTexture, UV).r;
	vec2 uv = UV + noise * 0.7f;
	vec4 colorA = texture(_WoodTexture, uv);
	vec4 colorB = texture(_ScarySmile, uv);
	vec3 color = mix(colorA.rgb, colorB.rgb, colorB.a * 0.3);

	FragColor = vec4(color, 1.0);
}