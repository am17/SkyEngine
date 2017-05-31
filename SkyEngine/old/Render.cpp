#include "stdafx.h"
#include "Render.h"
#include "D3D11Device.h"
#include "globals.h"
#include "RenderTexture.h"

namespace sky
{
	Render::Render(): 
		m_device(nullptr), 
		m_shaderManager(nullptr), 
		currentFps(0), 
		framePassed(0), 
		secondsPassed(0)
	{

	}


	Render::~Render()
	{
		if (m_shaderManager)
		{
			delete m_shaderManager;
			m_shaderManager = nullptr;
		}

		if (m_device)
		{
			m_device->close();

			delete m_device;
			m_device = nullptr;
		}
	}

	XMFLOAT4X4 Render::getProjectMatrix() const
	{
		return m_projectionMatrix;
	}

	void Render::bindRenderData(MeshRenderData *renderData) const
	{
		assert(renderData != nullptr);

		m_device->setPipelineState(renderData->getPipelineState());

		SDrawCall* drawCalls = renderData->getPipelineState()->getDrawCalls();
		size_t callsCount = renderData->getPipelineState()->getDrawCallsCount();
		
		m_device->draw(drawCalls, callsCount);
	}

	bool Render::initDevice(HWND hwnd, int screenWidth, int screenHeight, bool fullscreen)
	{
		m_device = new D3D11Device();

		if(!m_device->create(hwnd, screenWidth, screenHeight, false))
		{
			return false;
		}

		global::setDevice(m_device);

		m_shaderManager = new ShaderManager(m_device);

		assert(m_shaderManager != nullptr);

		global::setShaderManager(m_shaderManager);

		m_shaderManager->initShadersCollection();

		float fieldOfView = XM_PI / 4.0f;
		float screenAspect = m_device->getScreenAspectRatio();

		XMMATRIX projectionMat = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 1000.0f);

		XMStoreFloat4x4(&m_projectionMatrix, projectionMat);

		//RenderTexture *testText = new RenderTexture(1024, 1024);

		//m_device->setRenderTarget(testText->getRenderTargetResources());

		return true;
	}

	IDevice* Render::getDevice() const
	{
		return m_device;
	}

	ShaderManager* Render::getShaderManager() const
	{
		return m_shaderManager;
	}

	void Render::beginScene() const
	{
		m_device->beginScene();
	}

	void Render::endScene()
	{
		m_device->endScene();

		calcFps();
	}

	int Render::getFps() const
	{
		return currentFps;
	}

	void Render::calcFps()
	{
		Timer *timer = m_device->getTimer();

		/*static int frameCount = 0;
		static float timeElapsed = 0.0f;

		frameCount++;

		if ((timer->gameTime() - timeElapsed) >= 1.0f)
		{
			currentFps = (float)frameCount;
			float mspf = 1000.0f / currentFps;

			frameCount = 0;
			timeElapsed += 1.0f;
		}*/

		double currentTime = timer->getCurrentTime();
		double lastTime = timer->getLastTime();

		double delta = currentTime - lastTime;

		secondsPassed += delta;
		framePassed++;

		if (secondsPassed >= 1)
		{
			currentFps = framePassed;

			secondsPassed = 0;
			framePassed = 0;
		}
	}

	void Render::renderToTexture(IRenderTargetResources *renderTargetResources)
	{
		m_device->setRenderTarget(renderTargetResources);

		m_device->clearRenderTarget(renderTargetResources);

		//RenderScene()
		
		m_device->resetRenderTarget();
	}

	namespace global
	{
		ShaderManager *g_ShaderManager = nullptr;
	}
}
