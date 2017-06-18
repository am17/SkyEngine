#pragma once
#include "IDeviceImpl.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <unordered_map>

using namespace DirectX;
using namespace std;

class D3D11DeviceImpl : public IDeviceImpl
{
public:
	D3D11DeviceImpl();
	~D3D11DeviceImpl();
	bool init(HWND hwnd, int screenWidth, int screenHeight, bool fullscreen) override;
	void clearBackBuffer() override;
	void present() override;
	VertexBuffer* createVertexBuffer(const void *pData, unsigned int elementsCount, unsigned int stride) override;
	IndexBuffer* createIndexBuffer(const void *pData, size_t elementsCount, unsigned int stride, INDEX_BUFFER_FORMAT format) override;
	Shader* createVertexShader(const void* pByteCode, size_t ByteCodeLength) override;
	Shader* createPixelShader(const void* pByteCode, size_t ByteCodeLength) override;
	InputLayout* createInputLayout(VertexLayoutDesc *layoutDesc, const unsigned int layoutDescLength, const void* pByteCode, size_t ByteCodeLength) override;
	ConstantBuffer* createConstantBuffer(unsigned int bufferSize, EConstantBufferType type) override;
	Texture* createTextureFromFile(wchar_t* filename) override;
	Texture* createFresnelMap() override;
	void setPrimitiveTopology(PRIMITIVE_TOPOLOGY primitiveType) override;
	void setRepeatMaxAnisoSampler(sky::EShaderType shaderType, unsigned int slot) override;
	void setClampLinearSampler(sky::EShaderType shaderType, unsigned int slot) override;
	void setBackFaceCulling(bool cullBackFace) override;
	void setDepthState(sky::EDepthState depthState) override;
	void setBlendState(sky::EBlendState blendState) override;
	void clearRenderTarget(ERenderTarget renderTarget) override;
	void setRenderTarget(ERenderTarget renderTarget) override;
	Texture *getDepthMap() const override;
	void draw(unsigned int vertexCount, unsigned int startVertexIndex) override;
	void drawIndexed(unsigned int IndexCount, unsigned int StartIndexLocation, int  BaseVertexLocation) override;
	void drawIndexedInstanced(unsigned int indexCountPerInstance, unsigned int instanceCount, unsigned int startIndexLocation, int  baseVertexLocation, unsigned int startInstanceLocation) override;

	struct DispayModeRational
	{
		DispayModeRational() : numerator(0), denominator(1) {}
		unsigned int numerator;
		unsigned int denominator;
	};
private:
	bool fillVideoCardInfo(DXGI_FORMAT pixelFormat, bool vsyncEnabled);
	bool createDeviceAndSwapChain(HWND hwnd, DXGI_FORMAT pixelFormat, bool fullscreen);
	bool createDepthStencilBuffer(DXGI_FORMAT resourceDataFormat);
	bool createDepthStencilStates();
	bool createBackBufferRenderTarget(DXGI_FORMAT resourceDataFormat);
	bool createRasterizerStates();
	void setCullBackRasterizerState();
	void setNoCullBackRasterizerState();
	void setViewPort();
	void createSamplerState();
	void createBlendStates();
	bool fillVideoCardDescription(IDXGIAdapter* adapter);
	bool fillDispayModeRational(IDXGIAdapter* adapter, DXGI_FORMAT pixelFormat, bool vsyncEnabled);
	//bool createDepthMapRenderTarget();
	void testScene();

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView *m_backBufferRenderTarget;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11DepthStencilState* m_depthStencilStateLessEqual;
	ID3D11DepthStencilView* m_depthStencil;
	ID3D11RasterizerState* m_cullBackRasterState;
	ID3D11RasterizerState* m_noCullingrasterState;
	ID3D11SamplerState* m_samplerRepeatMaxAniso;
	ID3D11SamplerState* m_samplerClampLinear;
	ID3D11BlendState* m_alphaBlendState;
	//ID3D11RenderTargetView *_depthMapRenderTarget;

	char* m_cardName;
	DispayModeRational m_dispayModeRational;
	unsigned int m_dedicatedVideoMemory;
	int m_screenWidth;
	int m_screenHeight;
	bool m_vsync_enabled;

	ERenderTarget _currentRenderTarget;
	sky::EDepthState _currentDepthState;

	ID3D11ShaderResourceView * _depthMapTexture;
	Texture *_depthMap;



	//ID3D11RenderTargetView* currentRenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];

	//D3D11StateCache stateCache;
};

