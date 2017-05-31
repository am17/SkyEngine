struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float3 Direction;
	float SpecularPower;
	float4 SpecularColor;
	float3 halfWayVector;
	float padding;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
};

float4 calculateLambertModelLight(float3 normal, float4 textureColor, Material material, DirectionalLight light)
{
	float3 directionToLight = -light.Direction;

	float4 Ia = material.Ambient * light.Ambient;

	float lightIntensity = max(saturate(dot(directionToLight, normal)), 0.0);
	float4 Id = material.Diffuse * lightIntensity * light.Diffuse;

	return material.Emission + (Ia + Id) * textureColor;
}

float4 calculatePhongModelLight(float3 normal, float4 textureColor, float3 viewDirection, Material material, DirectionalLight light)
{
	float3 directionToLight = -light.Direction;

	float4 Ia = material.Ambient * light.Ambient;

	float lightIntensity = max(saturate(dot(directionToLight, normal)), 0.0);
	float4 Id = material.Diffuse * lightIntensity * light.Diffuse;//закон Ламберта

	/*float3 reflectDirection = reflect(-directionToLight, normal);
	float dotRV = saturate(dot(reflectDirection, viewDirection));
	float reflectionPower = pow(max(dotRV, 0.0), material.Shininess);*/

	//Упрощенный расчет зеркальной компоненты освещенности. Модель Блинна-Фонга
	float3 H = normalize(light.halfWayVector);
	float dotHN = saturate(dot(H, normal));
	float reflectionPower = pow(max(dotHN, 0.0), 64);

	float4 Is = material.Specular *  reflectionPower * light.SpecularColor;

	return material.Emission + (Ia + Id + Is) * textureColor;
}

float4 calculatePhongModelLight(float3 normal, float4 textureColor, float4 specTextureColor, float3 viewDirection, DirectionalLight light)
{
	float3 directionToLight = -light.Direction;

	float4 Ia = light.Ambient;

	float lightIntensity = max(saturate(dot(directionToLight, normal)), 0.0);
	float4 Id = lightIntensity * light.Diffuse;//закон Ламберта

	float3 reflectDirection = reflect(-directionToLight, normal);
	float dotRV = saturate(dot(reflectDirection, viewDirection));
	float reflectionPower = pow(max(dotRV, 0.0), light.SpecularPower);

	float4 Is = reflectionPower * specTextureColor * light.SpecularColor;

	return (Ia + Id + Is) * textureColor;
}

float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}