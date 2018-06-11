#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 camPos;

void main()
{
	float ambientStrenght = 0.1;
	vec3 ambient = ambientStrenght * lightColor;
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrenght = 0.5;
	vec3 viewDir = normalize(camPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrenght * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}
