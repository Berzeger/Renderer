#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transInvModel;

out vec4 FragPos;
out vec4 Normal;
out vec2 TexCoords;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = model * vec4(aPos, 1.0);
	Normal = normalize(transInvModel * vec4(aNormal, 1.0));
	TexCoords = aTexCoords;
}
