#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _WoodTexture, _ScarySmile, _OverlayTexture;
uniform float _Time, _Tiling;

void main(){
	vec2 tileUV = vec2(UV.x / _Tiling, UV.y / _Tiling); //scaling
	float noise = texture(_OverlayTexture, tileUV).r;
	vec2 uv = tileUV + noise * abs(sin(_Time));
	vec4 colorA = texture(_WoodTexture, uv);
	vec4 colorB = texture(_ScarySmile, uv);
	vec3 color = mix(colorA.rgb, colorB.rgb, colorB.a * 0.3);

	FragColor = vec4(color, 1.0);
}