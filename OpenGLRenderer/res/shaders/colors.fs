#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
  
uniform Material material;
uniform Light light;
uniform vec3 camPos;

void main()
{
	// ambient
	vec3 ambient = material.ambient * light.ambient;
	
	// diffuse
	vec3 norm = Normal;
	vec3 lightDir = normalize(light.position - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse) * light.diffuse;

	// specular
	vec3 viewDir = normalize(camPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * spec * light.specular;

	vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}
