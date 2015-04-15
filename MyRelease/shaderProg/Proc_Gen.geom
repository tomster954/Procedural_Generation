#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 frag_position[];
in vec3 frag_normal[];
in vec2 frag_texcoord[];

out vec4 gfrag_position;
out vec3 gfrag_normal;
out vec2 gfrag_texcoord;

uniform mat4 ProjectionView;

void main()
{
	//Get the new normals. :I

	vec3 position =  gl_in[0].gl_Position.xyz;
	vec3 position1 = gl_in[1].gl_Position.xyz;
	vec3 position2 = gl_in[2].gl_Position.xyz;

	gfrag_normal = cross(normalize(position1 - position), normalize(position2 - position));

	gfrag_texcoord = frag_texcoord[0];
	gfrag_position = frag_position[0];
	gl_Position = ProjectionView * gl_in[0].gl_Position;
	EmitVertex();

	gfrag_texcoord = frag_texcoord[1];
	gfrag_position = frag_position[1];
	gl_Position = ProjectionView * gl_in[1].gl_Position;
	EmitVertex();

	gfrag_texcoord = frag_texcoord[2];
	gfrag_position = frag_position[2];
	gl_Position = ProjectionView * gl_in[2].gl_Position;
	EmitVertex();
}