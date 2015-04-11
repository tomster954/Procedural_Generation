#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Colour;
layout(location=2) in vec2 texcoord;

out vec4 vColour;
out vec2 frag_texcoord;

uniform sampler2D perlin_texture;

uniform mat4 ProjectionView;

void main() 
{ 
	float scale = 100.0f;
	float waterLevel = 0.4f;
	
	
	vec4 pos = Position;
	
	if (texture(perlin_texture, texcoord).r < waterLevel)
		pos.y = waterLevel * scale;
	else
		pos.y += texture(perlin_texture, texcoord).r * scale;
		
	//Puts the map on 0 on the Y
	pos.y -= waterLevel * scale;
	
	//Moves the middle of the map to 0, y, 0
	pos = pos + vec4(-150, 0, -150, 0);
	
	frag_texcoord = texcoord;
	gl_Position = ProjectionView * pos; 
}