#pragma once
#include "IDevice.h"
#include "ShaderManager.h"
#include <DirectXMath.h>
#include "StaticMeshRenderData.h"


using namespace DirectX;

namespace sky
{
	class Render
	{
	public:
		SKYENGINEDLL Render();
		SKYENGINEDLL ~Render();
		XMFLOAT4X4 getProjectMatrix() const;
		void bindRenderData(MeshRenderData *renderData) const;
		SKYENGINEDLL bool initDevice(HWND hwnd, int screenWidth, int screenHeight, bool fullscreen);
		SKYENGINEDLL IDevice *getDevice() const;
		ShaderManager *getShaderManager() const;
		
		SKYENGINEDLL void beginScene() const;
		SKYENGINEDLL void endScene();
		SKYENGINEDLL int getFps() const;
		void renderToTexture(IRenderTargetResources *renderTargetResources);
	private:
		void calcFps();
		IDevice *m_device;
		XMFLOAT4X4 m_projectionMatrix;
		ShaderManager *m_shaderManager;
		int currentFps;
		int framePassed;
		double secondsPassed;
	};
}

