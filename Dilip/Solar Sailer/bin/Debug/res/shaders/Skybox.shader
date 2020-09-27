#shader vertex
#version 400 core

layout(location = 0) in vec3 position;
uniform mat4 u_mvp;
out vec3 v_TexCoord;

void main()
{
    gl_Position = u_mvp * vec4(position, 1.0);
    v_TexCoord = position;
}

//-----------------------------------------------

#shader fragment
#version 400 core

in vec3 v_TexCoord;

uniform samplerCube cubeMap;
uniform vec3 fogColor;

out vec4 color;

float lowerLimit = 00.0;
float UpperLimit = 12.0;

void main()
{
    color = texture(cubeMap, v_TexCoord);
}
