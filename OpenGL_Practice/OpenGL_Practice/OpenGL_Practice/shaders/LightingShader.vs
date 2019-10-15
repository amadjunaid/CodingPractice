#version 330 core
out vec2 TexCoord;

layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTexCoord; // the texcoord variable has attribute position 1

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()	
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}       