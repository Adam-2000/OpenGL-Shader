#version 330 core           //版本号 
out vec4 FragColor;         //输出颜色

//输入变量
in vec3 ourColor;           //顶点颜色
in vec2 TexCoord;           //UV坐标
in vec3 Normal;             //世界空间的法线向量
in vec3 WorldPosition;      //世界空间的顶点位置

/**********************************************************结构体属性声明*******************************************************/
//材质属性
struct Material
{
    vec3 ambientColor;    //环境光颜色              
    sampler2D diffuse;   //漫反射颜色       漫反射光照下物体的颜色。（和环境光照一样）漫反射颜色也要设置为我们需要的物体颜色
    sampler2D specular; //镜面光照          镜面光照对物体的颜色影响（或者甚至可能反射一个物体特定的镜面高光颜色）
    float shininess;   //反光度             影响镜面高光的散射/半径
};

//平行光结构体变量
struct DirLight
{
    vec3 DirLightDirection;         //平行光的方向
    
    vec3 DirLightColor;           //平行光的颜色
    float DirLightIntensity;     //平行光的强度
};

//点光源结构体变量
struct PointLight
{
    vec3 PointLightPosition;        //点光源的位置
    
    vec3 PointLightColor;         //点光源的颜色 
    float PointLightIntensity;   //点光源的强度
    
    float PLightconstant;      //衰减常量
    float PLightlinear;       //衰减线性一次项
    float PLightQuadratic;   //衰减平方二次项
};

//聚光灯结构体属性
struct SpotLight
{
    vec3 SpotLightPosition;      //位置
    vec3 SpotLightDirection;    //朝向
    float SpotInCutOff;        //内圆锥角
    float SpotOutCutOff;      //外圆锥角
    
    vec3 SpotLightColor;       //灯光颜色
    float SpotLightIntensity; //灯光强度
};

//声明Shader所用变量

#define NR_POINT_LIGHTS 2                          //两个点光源
uniform Material material;                        //材质属性结构体
uniform DirLight dirLight;                       //平行光属性结构体
uniform PointLight pointLight[NR_POINT_LIGHTS]; //点光源结构体
uniform SpotLight spotLight;                   //聚光灯结构体  

uniform float mixValue;                      //声明控制混合的变量
uniform vec3 viewPos;                       //摄像机的位置   

//函数的前置声明
vec3 CalcFengDiffuseShadingLight(vec3 lightDir, vec3 normal, vec3 viewDir);
vec3 CalcFengSpecularShadingLight(vec3 lightDir, vec3 normal, vec3 viewDir, float materialShininess);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor, Material material);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 worldPosition, vec3 viewDir, vec3 diffuseColor, vec3 specularColor, Material materia);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 worldPosition, vec3 viewDir, vec3 diffuseColor, vec3 specularColor, Material materia);


//片源着色主函数
void main()
{
    //法线 与 摄像机向量
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - WorldPosition);
    
    //采样贴图
    vec3 texDiffuse = texture(material.diffuse, TexCoord).rgb;
    vec3 texSpecular = texture(material.specular, TexCoord).rgb;
    
    //平行光权重值
    vec3 result = CalcDirLight(dirLight, N, V, texDiffuse, texSpecular,material);
    
    //点光源权重值
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLight[i], N, WorldPosition, V, texDiffuse, texSpecular, material);
    }
    
    //聚光灯权重值
    result += CalcSpotLight(spotLight, N, WorldPosition, V,texDiffuse, texSpecular,material);
    
    FragColor = vec4(material.ambientColor * texDiffuse+result, 1.0f);
}


/*******************************************************************函数*********************************************************/

//冯模型 灯光漫反射计算
vec3 CalcFengDiffuseShadingLight(vec3 lightDir, vec3 normal, vec3 viewDir)
{
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    return vec3(diff);
}
//冯模型 灯光镜面高光计算
vec3 CalcFengSpecularShadingLight(vec3 lightDir, vec3 normal, vec3 viewDir, float materialShininess)
{
    
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    
    return vec3(spec);
}
//平行光计算
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor, Material material)
{
    //计算灯光的方向向量
    vec3 lightDir = normalize(light.DirLightDirection);
    // 冯氏光照模型计算  
    vec3 diffuse = CalcFengDiffuseShadingLight(lightDir, normal, viewDir) * diffuseColor;
    vec3 specular = CalcFengSpecularShadingLight(lightDir, normal, viewDir, material.shininess) * specularColor;
   
    return (diffuse + specular) * light.DirLightColor * light.DirLightIntensity;
}
//点光源的计算
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 worldPosition, vec3 viewDir, vec3 diffuseColor, vec3 specularColor, Material materia)
{
    //计算灯光的方向向量
    vec3 lightDir = normalize(light.PointLightPosition - worldPosition);
    //点光源衰减
    float distance = length(light.PointLightPosition - worldPosition);
    float attenuation = 1.0 / (light.PLightconstant + light.PLightlinear * distance + light.PLightQuadratic * (distance * distance));
    
    // 冯氏光照模型计算  
    vec3 diffuse = CalcFengDiffuseShadingLight(lightDir, normal, viewDir) * diffuseColor;
    vec3 specular = CalcFengSpecularShadingLight(lightDir, normal, viewDir, material.shininess) * specularColor;
    
    return (diffuse + specular) * light.PointLightColor * light.PointLightIntensity * attenuation;
}
//聚光灯的计算
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 worldPosition, vec3 viewDir, vec3 diffuseColor, vec3 specularColor, Material materia)
{
    //计算灯光的方向向量
    vec3 lightDir = normalize(light.SpotLightPosition - worldPosition);
    // 冯氏光照模型计算  
    vec3 diffuse = CalcFengDiffuseShadingLight(lightDir, normal, viewDir) * diffuseColor;
    vec3 specular = CalcFengSpecularShadingLight(lightDir, normal, viewDir, material.shininess) * specularColor;
    //聚光灯衰减
    float theta = dot(lightDir, normalize(-light.SpotLightDirection)); //聚光灯方向与 片源之间夹角
    float epsilon = light.SpotInCutOff - light.SpotOutCutOff;
    float intensity = clamp((theta - light.SpotOutCutOff) / epsilon, 0.0, 1.0);
    
    return (diffuse + specular) * light.SpotLightColor * light.SpotLightIntensity * intensity;
}