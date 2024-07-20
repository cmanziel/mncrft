#version 330 core

// the pos attribute comes from the arrays of the faces, one at a time and in the order chosen in the faces array
layout(location = 0) in vec3 pos_top;
layout(location = 1) in vec3 pos_bottom;
layout(location = 2) in vec3 pos_left;
layout(location = 3) in vec3 pos_right;
layout(location = 4) in vec3 pos_front;
layout(location = 5) in vec3 pos_back;

layout(location = 6) in vec2 tex_coords_v1; // texture coordinates for vertex 1
layout(location = 7) in vec2 tex_coords_v2;
layout(location = 8) in vec2 tex_coords_v3;
layout(location = 9) in vec2 tex_coords_v4;
layout(location = 10) in vec2 tex_coords_v5;
layout(location = 11) in vec2 tex_coords_v6;

layout(location = 12) in vec4 model_col0;
layout(location = 13) in vec4 model_col1;
layout(location = 14) in vec4 model_col2;
layout(location = 15) in vec4 model_col3;

layout(location = 16) in int side;
layout(location = 17) in float pointedFlag;

layout(location = 18) in vec3 bar_top;
layout(location = 19) in vec3 bar_bottom;
layout(location = 20) in vec3 bar_left;
layout(location = 21) in vec3 bar_right;
layout(location = 22) in vec3 bar_front;
layout(location = 23) in vec3 bar_back;

uniform mat4 view;
uniform mat4 proj;

out vec2 out_texCoords;
out float out_isPointed;
out vec3 out_barycentric;
out float out_lightFactor;

vec2 tex_coords[6] = vec2[6](tex_coords_v1, tex_coords_v2, tex_coords_v3, tex_coords_v4, tex_coords_v5, tex_coords_v6);
vec3 vert_pos[6] = vec3[6](pos_top, pos_bottom, pos_left, pos_right, pos_front, pos_back);
vec3 bar_coords[6] = vec3[6](bar_top, bar_bottom, bar_left, bar_right, bar_front, bar_back);
float light_factors[6] = float[6](1.0, 1.0, 0.6, 1.0, 0.8, 0.8);

void main() {
	mat4 model = mat4(model_col0, model_col1, model_col2, model_col3);

	vec3 position = vert_pos[side];

	mat4 mvp = proj * view * model;

	gl_Position = mvp * vec4(position, 1.0);
	out_texCoords = tex_coords[gl_VertexID % 6];
	out_isPointed = pointedFlag;

	out_lightFactor = light_factors[side];

	out_barycentric = bar_coords[side];
}