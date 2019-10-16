#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 normal;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.1;
    
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.f);
	vec3 diffuse = diff*lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	float spec1 = max(dot(viewDir, reflectDir), 0.0);
	vec3 specular = specularStrength * spec1 * lightColor;


	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result,1.0);


}