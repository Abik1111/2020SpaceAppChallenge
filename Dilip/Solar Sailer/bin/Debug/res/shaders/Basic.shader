#shader vertex
#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_MVP;
uniform mat4 u_model;

out vec2 v_TexCoord;
out vec3 v_normal;
out vec3 v_fPos;

void main()
{
    gl_Position = u_MVP * vec4(position, 1.0);

    v_fPos = vec3(u_model * vec4(position, 1.0));
    v_TexCoord = texCoord;

    //Recalculating the normal but without translation effect
    mat4 model = u_model;
    model[3].x = 0;
    model[3].y = 0;
    model[3].z = 0;
    model[3].w = 1;
    v_normal = normalize(vec3(model * vec4(normal, 1.0)));
}


//-----------------------------------------------
#shader fragment
#version 400 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3  direction;
    float cutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform vec4 u_Color;
uniform vec3 u_viewPos;

uniform sampler2D u_Textures[2];
uniform int u_texSlot;
uniform int u_textureMap;
uniform vec3 u_color;

uniform Material material;
uniform Light light;
uniform DirectionalLight dlight;
uniform PointLight plight;
uniform SpotLight slight;
uniform int u_useLighting;
uniform int u_lightType;

in vec2 v_TexCoord;
in float v_texSlot;
in vec3 v_normal;
in vec3 v_fPos;

out vec4 color;

void main()
{
    //Use Color instead of texture if invalid slot is specified
    if(u_texSlot == -1)
    {
        //Simply use color if no lighting is to be used
        if(u_useLighting == 0)
        {
            color = vec4(u_color, 1.0);
        }
        else
        {
            vec3 ambient = light.ambient * material.ambient;

            vec3 norm = normalize(v_normal);
            vec3 lightDirection = normalize(light.position - v_fPos);
            float diff = max(dot(norm, lightDirection), 0.0);
            vec3 diffuse = light.diffuse * diff * material.diffuse;

            vec3 viewDir = normalize(u_viewPos - v_fPos);
            vec3 reflectDir = reflect(-lightDirection, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            vec3 specular = light.specular * (spec * material.specular);

            color = vec4((ambient + diffuse + specular)* u_color, 1.0);
        }
    }

    //Use texture of specified slot
    else
    {
        int index = u_texSlot;
        vec4 texColor = texture(u_Textures[index], v_TexCoord);

        if(texColor.a< 0.5)
            discard;

        //Simply use texture color if no lighting is to be used
        if(u_useLighting == 0)
        {
            color = texColor;
        }

        //If lighting is to be used
        else
        {
            //If texture mapping is not used
            if(u_textureMap == 0)
            {
                //Type 0 is point light without attenuation
                if(u_lightType == 0)
                {
                    vec3 ambient = light.ambient * material.ambient;

                    vec3 norm = normalize(v_normal);
                    vec3 lightDirection = normalize(light.position - v_fPos);
                    float diff = max(dot(norm, lightDirection), 0.0);
                    vec3 diffuse = light.diffuse * diff * material.diffuse;

                    vec3 viewDir = normalize(u_viewPos - v_fPos);
                    vec3 reflectDir = reflect(-lightDirection, norm);
                    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
                    vec3 specular = light.specular * (spec * material.specular);

                    color = vec4((ambient + diffuse + specular)* vec3(texColor), 1.0);
                }
                //Type 1 is directional light
                else if(u_lightType == 1)
                {
                    vec3 ambient = dlight.ambient * material.ambient;

                    vec3 norm = normalize(v_normal);
                    vec3 lightDirection = normalize(-dlight.direction);
                    float diff = max(dot(norm, lightDirection), 0.0);
                    vec3 diffuse = dlight.diffuse * diff * material.diffuse;

                    vec3 viewDir = normalize(u_viewPos - v_fPos);
                    vec3 reflectDir = reflect(-lightDirection, norm);
                    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
                    vec3 specular = dlight.specular * (spec * material.specular);

                    color = vec4((ambient + diffuse + specular)* vec3(texColor), 1.0);
                }
                //Type 2 is point light
                else if(u_lightType == 2)
                {
                    float distance    = length(plight.position - v_fPos);
                    float attenuation = 1.0 / (plight.constant + plight.linear * distance +
                                    plight.quadratic * (distance * distance));
                    vec3 ambient = plight.ambient * material.ambient;

                    vec3 norm = normalize(v_normal);
                    vec3 lightDirection = normalize(plight.position - v_fPos);
                    float diff = max(dot(norm, lightDirection), 0.0);
                    vec3 diffuse = plight.diffuse * diff * material.diffuse;

                    vec3 viewDir = normalize(u_viewPos - v_fPos);
                    vec3 reflectDir = reflect(-lightDirection, norm);
                    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
                    vec3 specular = plight.specular * (spec * material.specular);

                    color = vec4((attenuation*(ambient + diffuse + specular))* vec3(texColor), 1.0);
                }

            }

            //If texture is to be mapped
            else
            {
                vec3 ambient = light.ambient * vec3(texColor);

                vec3 norm = normalize(v_normal);
                vec3 lightDirection = normalize(light.position - v_fPos);
                float diff = max(dot(norm, lightDirection), 0.0);
                vec3 diffuse = light.diffuse * diff * vec3(texColor);

                vec3 viewDir = normalize(u_viewPos - v_fPos);
                vec3 reflectDir = reflect(-lightDirection, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
                vec3 specular = light.specular * (spec * vec3(texColor));

                color = vec4(ambient + diffuse + specular, 1.0);
            }
        }
    }
}
