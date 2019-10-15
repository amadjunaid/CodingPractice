#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 normal;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.f);
	vec3 diffuse = diff*lightColor;

	vec3 result = (ambient + diffuse) * objectColor;
	FragColor = vec4(result, 1.0);


}