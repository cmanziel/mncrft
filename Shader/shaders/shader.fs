#version 330 core

layout(location = 0) out vec4 color;
uniform sampler2D tex;
in vec2 out_texCoords;
in float out_isPointed;
in vec3 out_barycentric;
in float out_lightFactor;

void main() {
	if(any(lessThan(out_barycentric, vec3(0.02, 0.0, 0.02))) && out_isPointed == 1.0)
		color = vec4(0.0, 0.0, 0.0, 1.0);
	else
		color = texture(tex, out_texCoords) * out_lightFactor;
}