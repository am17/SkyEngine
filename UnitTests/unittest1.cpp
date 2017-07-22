#include "stdafx.h"
#include "CppUnitTest.h"
#include "D3D11Device.h"
#include "ShaderFactory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(InitDeviceTest)
		{
			IDeviceImpl* device = new D3D11Device();
			device->Init();
			device->Init();
		}

		TEST_METHOD(CreateTexture2DTest)
		{
			IDeviceImpl* device = new D3D11Device();
			device->Init();
			void *texture = device->CreateTexture2D(400, 400, nullptr, true, false);

			Assert::IsNotNull(texture, L"Текстура не создана");
		}

		TEST_METHOD(ShaderFactoryTest)
		{
			IDeviceImpl* device = new D3D11Device();
			device->Init();
			
			ShaderFactory &factory = ShaderFactory::GetInstance(device);
			ShaderFactory &factory2 = ShaderFactory::GetInstance(device);

			factory.GetShader(L"testVS", SHADER_TYPE::VERTEX_SHADER, "main");
			factory.GetShader(L"testPS", SHADER_TYPE::PIXEL_SHADER, "main");
			factory.GetShader(L"testDS", SHADER_TYPE::DOMAIN_SHADER, "main");
			factory.GetShader(L"testHS", SHADER_TYPE::HULL_SHADER, "main");
			factory.GetShader(L"testGS", SHADER_TYPE::GEOMETRY_SHADER, "main");
			factory.GetShader(L"testCS", SHADER_TYPE::COMPUTE_SHADER, "main");
		}


		TEST_METHOD(SamplerStateTest)
		{
			IDeviceImpl* device = new D3D11Device();
			device->Init();

			SamplerStateDesc SSDesc;
			SSDesc.Filter = TEXTURE_FILTER::ANISOTROPIC;
			SSDesc.TextureAddress = SAMPLER_ADDRESS_MODE::WRAP;
			SSDesc.MaxAnisotropy = 16;
			SSDesc.ComparisonFunction = COMPARISON_FUNCTION::ALWAYS;

			device->CreateSamplerState(SSDesc);

			SSDesc.Filter = TEXTURE_FILTER::ANISOTROPIC;
			SSDesc.TextureAddress = SAMPLER_ADDRESS_MODE::BORDER;
			SSDesc.MaxAnisotropy = 16;
			SSDesc.ComparisonFunction = COMPARISON_FUNCTION::LESS;

			device->CreateSamplerState(SSDesc);

			SSDesc.Filter = TEXTURE_FILTER::LINEAR;
			SSDesc.TextureAddress = SAMPLER_ADDRESS_MODE::WRAP;
			SSDesc.MaxAnisotropy = 16;
			SSDesc.ComparisonFunction = COMPARISON_FUNCTION::LESS;

			device->CreateSamplerState(SSDesc);

			SSDesc.Filter = TEXTURE_FILTER::LINEAR;
			SSDesc.TextureAddress = SAMPLER_ADDRESS_MODE::CLAMP;
			SSDesc.MaxAnisotropy = 16;
			SSDesc.ComparisonFunction = COMPARISON_FUNCTION::LESS;

			device->CreateSamplerState(SSDesc);

			SSDesc.Filter = TEXTURE_FILTER::POINT;
			SSDesc.TextureAddress = SAMPLER_ADDRESS_MODE::CLAMP;
			SSDesc.MaxAnisotropy = 16;
			SSDesc.ComparisonFunction = COMPARISON_FUNCTION::LESS;

			device->CreateSamplerState(SSDesc);
		}

		TEST_METHOD(ConstantBufferTest)
		{
			IDeviceImpl* device = new D3D11Device();
			device->Init();

			struct CBuffer
			{
				float		g_RenderCaustics;
				float		g_UseDynamicLOD;
				float		g_FrustumCullInHS;
				float       g_DynamicTessFactor;
			};

			unsigned int Size = sizeof(CBuffer);
			
			ConstantBuffer * buffer = device->CreateConstantBuffer(Size);

			CBuffer *Data = new CBuffer();

			Data->g_DynamicTessFactor = 1;
			Data->g_UseDynamicLOD = 2;
			Data->g_FrustumCullInHS = 3;
			Data->g_RenderCaustics = 4;

			buffer->UpdateData(Data, Size);
		}
	};
}