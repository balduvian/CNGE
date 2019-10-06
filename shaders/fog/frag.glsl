#version 330 core

uniform sampler2D tex;

uniform vec4 in_color;

uniform vec4 fog_color;

uniform vec3 player_pos;

uniform vec2 fog;

out vec4 color;

in vec2 tex_pass;
in vec3 world_pos;

void main()
{
	float dist = distance(world_pos, player_pos);

	float along = smoothstep(fog.x, fog.y, dist);

	vec4 tex_color = texture(tex, tex_pass) * in_color;

	if (tex_color.w < 0.5) { discard; }

	color = vec4(mix(tex_color, fog_color, along).xyz, tex_color.w);
}
