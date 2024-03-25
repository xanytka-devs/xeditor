#version 330 core
in vec2 UV;
out vec4 FragColor;

uniform sampler2D text;
uniform int render_mode;
uniform vec3 textColor;

void main() {
	vec4 t_sample = vec4(1.0, 1.0, 1.0, texture(text, UV).r);
	if(render_mode == 0) FragColor = vec4(textColor, 1.0) * t_sample;
	else if(render_mode == 1) FragColor = vec4(UV, 1.0, 1.0);
	else if(render_mode == 2) FragColor = vec4(textColor, 1.0);
}