#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "InputLayout.h"
#include "Texture.h"
#include "DepthStates.h"
#include "RenderTarget.h"

class IDeviceImpl
{
public:
	virtual ~IDeviceImpl(){}
	virtual bool init(HWND m_hwnd, int screenWidth, int screenHeight, bool fullscreen) = 0;
	virtual void clearBackBuffer() = 0;
	virtual void present() = 0;
	virtual VertexBuffer* createVertexBuffer(const void *pData, unsigned int elementsCount, unsigned int stride) = 0;
	virtual IndexBuffer* createIndexBuffer(const void *pData, size_t elementsCount, unsigned int stride, INDEX_BUFFER_FORMAT format) = 0;
	virtual Shader* createVertexShader(const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual Shader* createPixelShader(const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual InputLayout* createInputLayout(VertexLayoutDesc *layoutDesc, const unsigned int layoutDescLength, const void* pByteCode, size_t ByteCodeLength) = 0;
	virtual ConstantBuffer* createConstantBuffer(unsigned int bufferSize, EConstantBufferType type) = 0;
	virtual Texture* createTextureFromFile(wchar_t* filename) = 0;
	virtual Texture* createTexture2D(unsigned int width, unsigned int height, const void* data, bool createRenderTarget, unsigned int multiSampleCount = 1, unsigned int multiSampleQuality = 0) = 0;
	virtual void setPrimitiveTopology(PRIMITIVE_TOPOLOGY primitiveType) = 0;
	virtual void setRepeatMaxAnisoSampler(sky::EShaderType shaderType, unsigned int slot) = 0;
	virtual void setClampLinearSampler(sky::EShaderType shaderType, unsigned int slot) = 0;
	virtual void setBackFaceCulling(bool cullBackFace) = 0;
	virtual void setDepthState(sky::EDepthState depthState) = 0;
	virtual void setBlendState(sky::EBlendState blendState) = 0;
	virtual void setBackBufferAsRenderTarget() = 0;
	virtual void draw(unsigned int vertexCount, unsigned int startVertexIndex) = 0;
	virtual void drawIndexed(unsigned int IndexCount, unsigned int StartIndexLocation, int  BaseVertexLocation) = 0;
	virtual void drawIndexedInstanced(unsigned int indexCountPerInstance, unsigned int instanceCount, unsigned int startIndexLocation, int  baseVertexLocation, unsigned int startInstanceLocation) = 0;
};
