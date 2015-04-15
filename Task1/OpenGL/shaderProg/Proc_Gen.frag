#version 410

in vec4 vColour;

in vec2 frag_texcoord;
in vec4 frag_position;
in vec3 frag_normal;

out vec4 out_color;
uniform sampler2D perlin_texture;
uniform sampler2D rock_texture;
uniform sampler2D dirtGrass_texture;
uniform sampler2D water_texture;
uniform sampler2D snow_texture;

uniform sampler2D diffuse;
uniform vec3 LightDir = vec3(1, 1, 0);
uniform vec3 LightColour = vec3(1, 1, 1);
uniform vec3 AmbientColour = vec3(0.25, 0.25, 0.25);
uniform vec3 CameraPos;
uniform float SpecPow = 128;

void CalulateLight();

void main() 
{ 
	float textureScale = 30.0f;
	float height = texture(perlin_texture, frag_texcoord).r;
	
	float water_TextLevelBot = 0.00f;
	float water_TextLevelTop = 0.37f;

	float grass_TextLevelBot = 0.40f;
	float grass_TextLevelTop = 0.45f;
	
	float rock_TextLevelBot = 0.65f;
	float rock_TextLevelTop = 0.70f;
	
	float snow_TextLevelBot = 0.90f;
	float snow_TextLevelTop = 1.00f;
	
	//Setting the textures based of the height of the map
	
	//water
	if (height > water_TextLevelBot && height < water_TextLevelTop)
		out_color = texture(water_texture, vec2(frag_texcoord.x * textureScale, frag_texcoord.y * textureScale)).rgba;
	
	//blend between water and grass
	if (height > water_TextLevelTop && height < grass_TextLevelBot)
		out_color = mix(texture(water_texture, vec2(frag_texcoord.x * textureScale, frag_texcoord.y * textureScale)).rgba, texture(dirtGrass_texture, vec2(frag_texcoord.x * textureScale, frag_texcoord.y * textureScale)).rgba, (height - water_TextLevelTop) * 10);
	
	//grass
	if (height > grass_TextLevelBot && height < grass_TextLevelTop)
		out_color = texture(dirtGrass_texture, vec2(frag_texcoord.x * textureScale, frag_texcoord.y * textureScale)).rgba;
	
	//blend between grass and rock
	if (height > grass_TextLevelTop && height < rock_TextLevelBot)
		out_color = mix(texture(dirtGrass_texture, vec2(frag_texcoord.x * textureScale, frag_texcoord.y * textureScale)).rgba, texture(rock_texture, vec2(frag_texcoord.x * textureScale, frag_texcoord.y * textureScale)).rgba, (height - grass_TextLevelTop) * 5);
	
	//rock
	if (height > rock_TextLevelBot && height < rock_TextLevelTop)
		out_color = texture(rock_texture, vec2(frag_texcoord.x * textureScale, frag_texcoord.y * textureScale)).rgba;
	
	//blend between rock and snow
	if (height > rock_TextLevelTop && height < snow_TextLevelBot)
		out_color = mix(texture(rock_texture, vec2(frag_texcoord.x * textureScale, frag_texcoord.y * textureScale)).rgba, texture(snow_texture, vec2(frag_texcoord.x * textureScale, frag_texcoord.y * textureScale)).rgba, (height - rock_TextLevelTop) * 5);
	
	//snow
	if (height > snow_TextLevelBot)
		out_color = texture(snow_texture, vec2(frag_texcoord.x * textureScale, frag_texcoord.y * textureScale)).rgba;
		
	out_color.a = 1;
	
	CalulateLight();
}

void CalulateLight()
{
	float d = max(0, dot( normalize(frag_normal.xyz), normalize(LightDir) ) );
	vec3 E = normalize( CameraPos - frag_position.xyz);
	vec3 R = reflect( -LightDir, frag_normal.xyz );
	float s = max( 0, dot( E, R ) );
	s = pow( s, SpecPow );
	out_color *= vec4(LightColour * d + LightColour * s, 1) + vec4(AmbientColour, 1);
}

















