#version 330 core

layout(location = 0) out vec4 color;
uniform sampler2D tex;
in vec2 out_texCoords;

void main() {
	//color = vec4(1.0, 0.0, 0.0, 1.0);
	
	color = texture(tex, out_texCoords);
}