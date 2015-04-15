#version 410
	
layout(location=0) in vec4 Position;				
layout(location=1) in vec4 Normal;					
layout(location=2) in vec4 tangent;					
layout(location=3) in vec2 texcoord;				
layout(location=4) in vec4 weights;					
layout(location=5) in vec4 indices;

out vec3 frag_normal;								
out vec3 frag_position;								
out vec3 frag_tangent;								
out vec3 frag_bitangent;							
out vec2 frag_texcoord;	

const int MAX_BONES = 128;							
							
uniform mat4 ProjectionView;			
uniform vec4 myTransform;			
uniform mat4 global;
uniform mat4 bones[MAX_BONES];						

void main()
{
	float xScale = 0.1f;
	float yScale = 0.1f;
	float zScale = 0.1f;
	
	vec4 FBX_Scale = vec4(xScale, yScale, zScale, 1);
	
	vec4 transform = vec4(0, 50, 0, 0);
	//vec4 transform = leltransform;
	
	frag_position = Position.xyz;					
	frag_normal = Normal.xyz;						
	frag_tangent = tangent.xyz;						
	frag_bitangent = cross(Normal.xyz, tangent.xyz);
	frag_texcoord = texcoord;	
	
	ivec4 index = ivec4(indices);					
	
	vec4 P = bones[ index.x ] * Position * weights.x;
	
	P += bones[ index.y ] * Position * weights.y;	
	P += bones[ index.z ] * Position * weights.z;	
	P += bones[ index.w ] * Position * weights.w;	
	P.w = 1;										
	
	gl_Position = ProjectionView * (global * (P * FBX_Scale) + myTransform);		
};