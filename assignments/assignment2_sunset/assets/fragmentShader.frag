	#version 450
	out vec4 FragColor;
	in vec2 UV;
	uniform vec2 _Resolution;
	uniform float _Time;

	float draw_circle(vec2 coord, float radius) {
		return smoothstep(radius, radius - 15.0 / _Resolution.y, length(coord));
	}

	void main(){
		   vec3 color = mix(vec3(1.0, 0.3, 0.0), vec3(0.2, 0.2, 0.7), UV.y); //sky gradient

		   vec2 scale = vec2(_Resolution.x / _Resolution.y, 1.0);
		   vec2 coord = UV.xy * scale;
		   vec2 offset = vec2(0.5, 0.5) * scale;
		   float circle = draw_circle(coord - offset, 0.15);

		   //sun color change
		   vec4 circleColor1 = vec4(0.9,0.7,0.0,1.0);
		   vec4 circleColor2 = vec4(1.0,0.5,0.0,1.0);
		   vec3 circleColor = mix(circleColor1.rgb, circleColor2.rgb, abs(sin(_Time)));

		   color = mix(color, circleColor.rgb, circle * circleColor1.a); //draw sun

		   float hills = 1.0 - step(sin(UV.x * 7.0) * 0.1 + 0.35, UV.y);
		   color = mix(color, vec3(0.1), hills); //draw hills

		 FragColor = vec4(color, 1.0);
	}