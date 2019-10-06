#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 tex_coord;

uniform mat4 model;
uniform mat4 proj_view;
uniform vec4 tex_modif;

out vec3 world_pos;
out vec2 tex_pass;

void main()
{
	gl_Position = (proj_view * model) * vec4(vertex, 1);

	tex_pass = (tex_coord * tex_modif.xy) + tex_modif.zw;

	world_pos = (model * vec4(vertex, 1)).xyz;
}
