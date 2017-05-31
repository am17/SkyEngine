#pragma once
#include "IDevice.h"
#include "ShaderManager.h"

namespace sky
{
	namespace global
	{
		extern IDevice *g_Device;
		inline IDevice* getDevice()
		{
			return g_Device;
		}

		inline void setDevice(IDevice* device)
		{
			g_Device = device;
		}

		extern ShaderManager *g_ShaderManager;
		inline ShaderManager* getShaderManager()
		{
			return g_ShaderManager;
		}

		inline void setShaderManager(ShaderManager* shaderManager)
		{
			g_ShaderManager = shaderManager;
		}
	}
}