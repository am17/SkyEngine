struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float3 Direction;
	float SpecularPower;
	float4 SpecularColor;
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

	// добавим фоновое освещение
	float4 Ia = material.Ambient * light.Ambient;

	// добавим рассеянный свет
	float lightIntensity = max(saturate(dot(directionToLight, normal)), 0.0);
	float4 Id = material.Diffuse * lightIntensity * light.Diffuse;//закон Ламберта

	return material.Emission + (Ia + Id) * textureColor;
}

float4 calculatePhongModelLight(float3 normal, float4 textureColor, float3 viewDirection, Material material, DirectionalLight light)
{
	float3 directionToLight = -light.Direction;

	// добавим фоновое освещение
	float4 Ia = material.Ambient * light.Ambient;

	// добавим рассеянный свет
	float lightIntensity = max(saturate(dot(directionToLight, normal)), 0.0);
	float4 Id = material.Diffuse * lightIntensity * light.Diffuse;//закон Ламберта

	// добавим отраженный свет (Модель отражения Фонга)
	float3 reflectDirection = reflect(-directionToLight, normal);
	float dotRV = saturate(dot(reflectDirection, viewDirection));
	float reflectionPower = pow(max(dotRV, 0.0), material.Shininess);

	//Упрощенный расчет зеркальной компоненты освещенности. Модель Блинна-Фонга
	//float3 H = normalize(directionToLight + reflectDirection);
	//float dotHN = saturate(dot(H, normal));
	//float reflectionPowersFast = pow(max(dotHN, 0.0), materialShininess);

	float4 Is = material.Specular *  reflectionPower * light.SpecularColor;

	return material.Emission + (Ia + Id + Is) * textureColor;
}

float4 calculateDirectionLight2(float3 normal, float4 textureColor, float3 viewDirection, Material material, DirectionalLight light)
{
	float3 directionToLight = -light.Direction;

	float4 result = light.Ambient * material.Ambient;

	float lightIntensity = max(saturate(dot(directionToLight, normal)), 0.0);

	result += material.Diffuse * light.Diffuse * lightIntensity;

	result = saturate(result);

	float reflectionPower = max(pow(saturate(dot(normalize(2 * lightIntensity * normal - directionToLight), viewDirection)), material.Shininess), 0.0);

	float4 specular = material.Specular * light.SpecularColor * reflectionPower;

	result = result * textureColor;

	result = saturate(result + specular);

	return result;
}