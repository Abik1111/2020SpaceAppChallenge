#shader vertex
#version 400 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCord;

out vec2 v_TexCoord;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    v_TexCoord = texCord;
}

//-----------------------------------------------

#shader fragment
#version 400 core

in vec2 v_TexCoord;

uniform sampler2D u_slot;

out vec4 color;

void main()
{
    float near = 0.1;
    float far = 300;

    color = texture(u_slot, v_TexCoord);
    //float distance = 2*near*far/(near+far-(2*color.r-1.0)*(far-near));
    //color = vec4(vec3(color.r), 1.0);
}
