#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 voutNormal;
in mat3 TBN;
in vec4 FragPos;
in vec4 eyePos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_height1;
uniform sampler2D texture_ambient1;
uniform sampler2D texture_shininess1;
uniform sampler2D texture_opacity1;

uniform bool flag_diffuse;
uniform bool flag_specular;
uniform bool flag_height;
uniform bool flag_ambient;
uniform bool flag_shininess;
uniform bool flag_opacity;


uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Ns;
uniform float d;

uniform vec3 lightColor1;
uniform vec4 lightDir1;
uniform vec3 lightColor2;
uniform vec4 lightDir2;
uniform vec3 lightColor3;
uniform vec4 lightDir3;

uniform vec3 ambientLight;

void main()
{
    vec4 Normal;
    vec3 normal;
    float N_exp;
    if(flag_opacity){
        FragColor.a = d * texture(texture_opacity1, TexCoords).r;
    } else {
        FragColor.a = d;
    }
    if(FragColor.a < 0.05) discard;
    if(flag_height){
        normal = texture(texture_height1, TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);   
        normal = normalize(TBN * normal);
        Normal = vec4(normal, 1.0);
    } else {
        Normal = voutNormal;
    }

    if(flag_shininess){
        N_exp = Ns * texture(texture_shininess1, TexCoords).r;
    } else{
        N_exp = Ns;
    }


    if(flag_ambient){
        FragColor.rgb = Ka * texture(texture_ambient1, TexCoords).rgb * ambientLight;
    } else {
        FragColor.rgb = Ka * ambientLight;
    }
    
    float cos = -dot(lightDir1, Normal);
    if(cos > 0){
        if(flag_diffuse){
            FragColor.rgb += Kd * texture(texture_diffuse1, TexCoords).rgb * cos * lightColor1;
        } else {
            FragColor.rgb += Kd * cos * lightColor1;
        }
    }
    vec4 p2eye = eyePos - FragPos;
    p2eye = normalize(p2eye);
    vec4 halfvec = p2eye - lightDir1;
    halfvec = normalize(halfvec);
    cos = dot(halfvec, Normal);
    if(cos > 0){
        if(flag_specular){
            FragColor.rgb += Ks * texture(texture_specular1, TexCoords).rgb * pow(cos, N_exp) * lightColor1;
        } else {
            FragColor.rgb += Ks * pow(cos, N_exp) * lightColor1;
        }
    }


    cos = -dot(lightDir2, Normal);
    if(cos > 0){
        if(flag_diffuse){
            FragColor.rgb += Kd * texture(texture_diffuse1, TexCoords).rgb * cos * lightColor2;
        } else {
            FragColor.rgb += Kd * cos * lightColor2;
        }
    }
    halfvec = normalize(p2eye - lightDir2);
    cos = dot(halfvec, Normal);
    if(cos > 0){
        if(flag_specular){
            FragColor.rgb += Ks * texture(texture_specular1, TexCoords).rgb * pow(cos, N_exp) * lightColor2;
        } else {
            FragColor.rgb += Ks * pow(cos, N_exp) * lightColor2;
        }
    }

    cos = -dot(lightDir3, Normal);
    if(cos > 0){
        if(flag_diffuse){
            FragColor.rgb += Kd * texture(texture_diffuse1, TexCoords).rgb * cos * lightColor3;
        } else {
            FragColor.rgb += Kd * cos * lightColor3;
        }
    }
    halfvec = normalize(p2eye - lightDir3);
    cos = dot(halfvec, Normal);
    if(cos > 0){
        if(flag_specular){
            FragColor.rgb += Ks * texture(texture_specular1, TexCoords).rgb * pow(cos, N_exp) * lightColor3;
        } else {
            FragColor.rgb += Ks * pow(cos, N_exp) * lightColor3;
        }
    }
}