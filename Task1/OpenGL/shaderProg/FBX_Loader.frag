#version 410

in vec3 frag_normal;
in vec3 frag_position;
in vec3 frag_tangent;
in vec3 frag_bitangent;
in vec2 frag_texcoord;

out vec4 FragColor;

uniform sampler2D diffuse;
uniform vec3 LightDir = vec3(1, 1, 0);
uniform vec3 LightColour = vec3(1, 1, 1);
uniform vec3 CameraPos;
uniform float SpecPow = 128;

void main()
{
	float d = max(0, dot( normalize(frag_normal.xyz), normalize(LightDir) ) );
	vec3 E = normalize( CameraPos - frag_position.xyz);
	vec3 R = reflect( -LightDir, frag_normal.xyz );
	float s = max( 0, dot( E, R ) );
	s = pow( s, SpecPow );
	FragColor = texture(diffuse, frag_texcoord) * vec4(LightColour * d + LightColour * s, 1);
};