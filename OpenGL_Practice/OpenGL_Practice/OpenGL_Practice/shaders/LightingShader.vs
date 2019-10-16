#version 330 core
out vec3 FragPos;
out vec3 normal;

layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal; // the texcoord variable has attribute position 1

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()	
{	
	FragPos = vec3(model * vec4(aPos, 1.0));
	normal = mat3(transpose(inverse(model)))*aNormal;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}       