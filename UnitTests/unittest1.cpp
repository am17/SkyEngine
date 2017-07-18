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
			
			ShaderFactory *factory = new ShaderFactory(device);
			factory->GetShader(L"mainVS", SHADER_TYPE::VERTEX_SHADER, "main");
		}
	};
}