#version 410

in vec4 vColour;
out vec4 FragColor;

in vec2 frag_texcoord;
out vec4 out_color;
uniform sampler2D perlin_texture;
uniform sampler2D rock_texture;
uniform sampler2D dirtGrass_texture;
uniform sampler2D water_texture;
uniform sampler2D snow_texture;


void main() 
{ 
	float height = texture(perlin_texture, frag_texcoord).r;
	
	//Setting the textures based of the height of the map
	if (height < 0.3f)
		out_color = texture(water_texture, vec2(frag_texcoord.x * 10, frag_texcoord.y * 10)).rgba;
	else if (height > 0.4f && height < 0.5)
		out_color = texture(dirtGrass_texture, frag_texcoord).rgba;
	else if (height > 0.3f && height < 0.4f)
		out_color = mix(texture(water_texture, vec2(frag_texcoord.x * 10, frag_texcoord.y * 10)).rgba, texture(dirtGrass_texture, frag_texcoord).rgba, (height - 0.3f) * 10);
	
	if (height > 0.6f)
		out_color = texture(rock_texture, frag_texcoord).rgba;
	else if (height > 0.5f && height < 0.6f)
		out_color = mix(texture(dirtGrass_texture, frag_texcoord).rgba, texture(rock_texture, frag_texcoord).rgba, (height - 0.5f) * 10);
	
	
	//if (height < 0.7f && height > 0.5f)
	//	out_color = texture(dirtGrass_texture1, frag_texcoord).rgba * vec4(1, 1, 1, 1);
	//else if (height > 0.8f)
	//	out_color = texture(rock_texture1, frag_texcoord).rgba * vec4(1, 1, 1, 1);
	//else if (height < 0.8f && height > 0.7f)
	//	out_color = mix(texture(dirtGrass_texture1, frag_texcoord).rgba, texture(rock_texture1, frag_texcoord).rgba, (height - 0.7) * 10);
		
	out_color.a = 1;
}