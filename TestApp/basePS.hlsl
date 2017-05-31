//#include "lightingModel.fx"

Texture2D shaderTexture: register(t0);
SamplerState SampleType: register(s0);

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float3 Direction;
	float SpecularPower;
	float4 SpecularColor;
	float3 halfWayVector;
	float textureTranslation;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float3 Position;
	float SpecularPower;
	float4 SpecularColor;
	float3 halfWayVector;
	float Range;
	float3 Attenuation;
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

cbuffer cbPerFrame: register(b0)
{
	DirectionalLight dirLight;
	//PointLight dirLight;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

static Material material;

float4 calculatePhongModelLight(float3 normal, float4 textureColor, float3 viewDirection, Material material, DirectionalLight light)
{
	float3 directionToLight = -light.Direction;

	// добавим фоновое освещение
	float4 Ia = material.Ambient * light.Ambient;

	// добавим рассеянный свет
	float lightIntensity = max(saturate(dot(directionToLight, normal)), 0.0);
	float4 Id = material.Diffuse * lightIntensity * light.Diffuse;//закон Ламберта

	// добавим отраженный свет (Модель отражения Фонга)
	//float3 reflectDirection = reflect(-directionToLight, normal);
	//float dotRV = saturate(dot(reflectDirection, viewDirection));
	//float reflectionPower = pow(max(dotRV, 0.0), material.Shininess);

	//Упрощенный расчет зеркальной компоненты освещенности. Модель Блинна-Фонга
	float3 H = normalize(light.halfWayVector);
	float dotHN = saturate(dot(H, normal));
	float reflectionPower = pow(max(dotHN, 0.0), 64);

	float4 Is = material.Specular *  reflectionPower * light.SpecularColor;

	return material.Emission + (Ia + Id + Is) * textureColor;
}

float4 calculatePhongModelLight(float3 normal, float4 textureColor, float3 viewDirection, Material material, PointLight light)
{
	float3 directionToLight = -light.Position;

	// добавим фоновое освещение
	float4 Ia = material.Ambient * light.Ambient;

	// добавим рассеянный свет
	float lightIntensity = max(saturate(dot(directionToLight, normal)), 0.0);
	float4 Id = material.Diffuse * lightIntensity * light.Diffuse;//закон Ламберта

	// добавим отраженный свет (Модель отражения Фонга)
	//float3 reflectDirection = reflect(-directionToLight, normal);
	//float dotRV = saturate(dot(reflectDirection, viewDirection));
	//float reflectionPower = pow(max(dotRV, 0.0), material.Shininess);

	//Упрощенный расчет зеркальной компоненты освещенности. Модель Блинна-Фонга
	float3 H = normalize(light.halfWayVector);
	float dotHN = saturate(dot(H, normal));
	float reflectionPower = pow(max(dotHN, 0.0), 64);

	float4 Is = material.Specular *  reflectionPower * light.SpecularColor;

	return material.Emission + (Ia + Id + Is) * textureColor;
}

float4 calculateLambertModelLight(float3 normal, float4 textureColor, Material material, DirectionalLight light)
{
	float3 directionToLight = -light.Direction;

		float4 Ia = material.Ambient * light.Ambient;

		float lightIntensity = max(saturate(dot(directionToLight, normal)), 0.0);
	float4 Id = material.Diffuse * lightIntensity * light.Diffuse;

		return material.Emission + (Ia + Id) * textureColor;
}

float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor = shaderTexture.Sample(SampleType, input.tex);

	material.Ambient = float4(1.0, 1.0, 1.0, 0);
	material.Diffuse = float4(1.0, 1.0, 1.0, 0);
	material.Specular = float4(1.0, 1.0, 1.0, 0);
	material.Emission = float4(0, 0, 0, 0);
	material.Shininess = 0;

	return calculatePhongModelLight(input.normal, textureColor, input.viewDirection, material, dirLight);
}

float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}