#pragma once;
#include "Timer.h"
#include "D3D11PipelineState.h"
#include "ITexture.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"
using namespace DirectX;

namespace sky
{
	class IDevice
	{
	public:
		virtual ~IDevice() {}
		virtual bool create(HWND hwnd, int screenWidth, int screenHeight, bool fullscreen) = 0;
		virtual void close() = 0;
		virtual void beginScene() = 0;
		virtual void endScene() = 0;
		virtual Timer* getTimer() = 0;
		virtual int getScreenWidth() = 0;
		virtual int getScreenHeight() = 0;
		virtual float getScreenAspectRatio() = 0;
		virtual IAPIBuffer* createVertexBuffer(const void *pData, size_t elementsCount, unsigned int stride) = 0;
		virtual IAPIBuffer* createIndexBuffer(const void *pData, unsigned int elementsCount, unsigned int stride) = 0;
		virtual IAPIBuffer* createConstantBuffer(unsigned int bufferSize, bool dynamic = false) = 0;
		virtual IVertexShader* createVertexShader(const void* pByteCode, size_t ByteCodeLength, size_t constantBufferCount, size_t vertexBufferCount) = 0;
		virtual IPixelShader* createPixelShader(const void* pByteCode, size_t ByteCodeLength, size_t constantBufferCount, size_t shaderResourceCount) = 0;
		virtual VertexInputLayout* getInputLayout(VertexLayoutDesc *layoutDesc, const unsigned int layoutDescLength, const void* pByteCode, size_t ByteCodeLength) = 0;
		virtual void setPipelineState(D3D11PipelineState *renderTmlt) = 0;
		virtual void draw(SDrawCall* drawCalls, size_t callCount) = 0;
		virtual IShaderResource *createShaderResource(WCHAR* filename) = 0;
		virtual IRenderTargetResources* createRenderTargetResources(int textureWidth, int textureHeight) = 0;
		virtual void setRenderTarget(IRenderTargetResources* renderTargetResources) = 0;
		virtual void resetRenderTarget() = 0;
		virtual void clearRenderTarget(IRenderTargetResources* renderTargetResources) = 0;
	};
}