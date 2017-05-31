#pragma once
#include <d3d11.h>
#include "IDevice.h"
#include <map>
#include <unordered_map>
#include "D3D11StateCache.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "SimpleMath.h"

using namespace std;
using namespace DirectX;

namespace sky
{
	class D3D11Device : public IDevice
	{
	public:
		SKYENGINEDLL D3D11Device();
		~D3D11Device();

		SKYENGINEDLL bool create(HWND hwnd, int screenWidth, int screenHeight, bool fullscreen) override;
		SKYENGINEDLL void close() override;
		SKYENGINEDLL void beginScene() override;
		SKYENGINEDLL void endScene() override;
		SKYENGINEDLL Timer* getTimer() override;
		int getScreenWidth() override;
		int getScreenHeight() override;
		float getScreenAspectRatio() override;
		IAPIBuffer* createVertexBuffer(const void *pData, size_t elementsCount, unsigned int stride) override;
		IAPIBuffer* createIndexBuffer(const void *pData, unsigned int elementsCount, unsigned int stride) override;
		IAPIBuffer* createConstantBuffer(unsigned int bufferSize, bool dynamic = false) override;
		IVertexShader* createVertexShader(const void* pByteCode, size_t ByteCodeLength, size_t constantBufferCount, size_t vertexBufferCount) override;
		IPixelShader* createPixelShader(const void* pByteCode, size_t ByteCodeLength, size_t constantBufferCount, size_t shaderResourceCount) override;
		IShaderResource* createShaderResource(WCHAR* filename) override;
		IRenderTargetResources* createRenderTargetResources(int textureWidth, int textureHeight) override;
		VertexInputLayout* getInputLayout(VertexLayoutDesc *layoutDesc, const unsigned int layoutDescLength, const void* pByteCode, size_t ByteCodeLength) override;
		void setPipelineState(D3D11PipelineState *renderTmlt) override;
		void draw(SDrawCall* drawCalls, size_t callCount) override;
		void setRenderTarget(IRenderTargetResources* renderTargetResources) override;
		void resetRenderTarget() override;
		void clearRenderTarget(IRenderTargetResources* renderTargetResources) override;
		
		struct DispayModeRational
		{
			DispayModeRational() : numerator(0), denominator(1) {}
			unsigned int numerator;
			unsigned int denominator;
		};
	private:
		bool fillVideoCardDescription(IDXGIAdapter* adapter);
		bool fillDispayModeRational(IDXGIAdapter* adapter, DXGI_FORMAT pixelFormat, bool vsyncEnabled);
		bool fillVideoCardInfo(DXGI_FORMAT pixelFormat, bool vsyncEnabled);
		bool createDeviceAndSwapChain(HWND hwnd, DXGI_FORMAT pixelFormat, bool fullscreen);
		void setViewPort();
		bool createRasterizerStates();
		void setCullBackRasterizerState();
		bool createBackBufferRenderTarget(DXGI_FORMAT resourceDataFormat);
		bool createDepthStencilState();
		void setDepthStencilState();
		bool createDepthStencilBuffer(DXGI_FORMAT resourceDataFormat);
		void setPrimitiveTopology(PRIMITIVE_TOPOLOGY PrimitiveType);
		void createSamplerState();
		void turnZBuffer(bool zBufferEnabled = true);
		void turnBackfaceCulling(bool cullBackface = true);
		void initText();
		void drawText() const;
		//API specific
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		IDXGISwapChain* m_swapChain;
		ID3D11RenderTargetView* m_backBufferRenderTarget;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilState* m_depthDisabledStencilState;
		ID3D11DepthStencilView* m_depthStencil;
		ID3D11RasterizerState* m_cullBackRasterState;
		ID3D11RasterizerState* m_noCullingrasterState;
		ID3D11SamplerState* m_sampleState;

		//custom
		char* m_cardName;
		unsigned int m_dedicatedVideoMemory;
		DispayModeRational m_dispayModeRational;
		bool m_vsync_enabled;
		Timer *m_timer;
		int m_screenWidth;
		int m_screenHeight;
		unordered_map<DWORD, VertexInputLayout*>  m_inputLayouts;
		D3D11StateCache stateCache;

		D3D11PipelineState *m_currentState;

		std::unique_ptr<DirectX::SpriteFont> m_font;
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
		DirectX::SimpleMath::Vector2 m_fontPos;

		//TArray<TRefCountPtr<FD3D11ConstantBuffer> > VSConstantBuffers;
		//TArray<TRefCountPtr<FD3D11ConstantBuffer> > HSConstantBuffers;
		//TArray<TRefCountPtr<FD3D11ConstantBuffer> > DSConstantBuffers;
		//TArray<TRefCountPtr<FD3D11ConstantBuffer> > PSConstantBuffers;
		//TArray<TRefCountPtr<FD3D11ConstantBuffer> > GSConstantBuffers;
		//TArray<TRefCountPtr<FD3D11ConstantBuffer> > CSConstantBuffers;

		
	};
}


