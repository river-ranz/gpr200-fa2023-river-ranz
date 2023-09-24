	#version 450
	out vec4 FragColor;
	in vec2 UV;
	uniform float _Time, _SunRadius, _SunSpeed;
	uniform vec2 _Resolution;
	uniform vec3 _SkyDayColorBottom, _SkyDayColorTop, _SkyNightColorBottom, _SkyNightColorTop, _SunYellow, _SunOrange, _HillColor;

	float draw_circle(vec2 coord, float radius) {
		return smoothstep(radius, radius - 15.0 / _Resolution.y, length(coord));
	}

	void main(){
		   //sky color change
		   vec3 colorDay = mix(_SkyDayColorTop, _SkyDayColorBottom, UV.y);
           vec3 colorNight = mix(_SkyNightColorTop, _SkyNightColorBottom, UV.y);
           vec3 color = mix(colorNight, colorDay, _SunSpeed);

		   //circle
		   vec2 scale = vec2(_Resolution.x / _Resolution.y, 1.0);
		   vec2 coord = UV.xy * scale;
		   vec2 offset = vec2(0.5, _SunSpeed - 0.4) * scale;
		   float circle = draw_circle(coord - offset, _SunRadius);

		   //sun color change
		   vec3 circleColor = mix(_SunOrange, _SunYellow, _SunSpeed);

           //draw sun
		   color = mix(color, circleColor.rgb, circle * 1.0);

           //draw hills
		   float hills = 1.0 - step(sin(UV.x * 5.5) * 0.1 + 0.3, UV.y);
		   color = mix(color, _HillColor, hills);

		 FragColor = vec4(color, 1.0);
	}