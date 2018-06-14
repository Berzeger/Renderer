#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emissive;
	float shininess;
};

struct Light {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 vector;
};

out vec4 FragColor;
in vec4 Normal;
in vec4 FragPos;
in vec2 TexCoords;
  
uniform Material material;
uniform Light light;
uniform vec4 camPos;
uniform float time;	

void main()
{
	// ambient
	vec4 ambient = light.ambient * texture(material.diffuse, TexCoords).rgba;
	
	// diffuse
	vec4 norm = Normal;
	vec4 lightDir;
	if (abs(light.vector.w) < 0.0001) // direction
	{
		lightDir = normalize(-light.vector);
	}
	else // position
	{
		lightDir = normalize(light.vector - FragPos);
	}

	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgba;

	// specular
	vec4 viewDir = normalize(camPos - FragPos);
	vec4 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec4 specular = light.specular * spec * texture(material.specular, TexCoords).rgba;

	// emissive
	vec4 emissive = texture(material.emissive, TexCoords).rgba * (sin(time) * 0.5 + 0.5);

    FragColor = ambient + diffuse + specular + emissive;
}
