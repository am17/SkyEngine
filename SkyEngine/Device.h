#pragma once
#include "IDeviceImpl.h"
#include "common.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "InputLayout.h"
#include "ConstantBuffer.h"

class Device
{
public:
	SKYENGINEDLL Device();
	SKYENGINEDLL ~Device();
	SKYENGINEDLL bool init(HWND m_hwnd, int screenWidth, int screenHeight, bool fullscreen);
	SKYENGINEDLL void clearBackBuffer();
	SKYENGINEDLL void present();
	VertexBuffer* createVertexBuffer(const void *pData, unsigned int elementsCount, unsigned int stride);
	IndexBuffer* createIndexBuffer(const void *pData, size_t elementsCount, unsigned int stride, INDEX_BUFFER_FORMAT format);
	Shader* createVertexShader(const void* pByteCode, size_t ByteCodeLength);
	Shader* createPixelShader(const void* pByteCode, size_t ByteCodeLength);
	InputLayout* createInputLayout(VertexLayoutDesc *layoutDesc, const unsigned int layoutDescLength, const void* pByteCode, size_t ByteCodeLength);
	ConstantBuffer* createConstantBuffer(unsigned int bufferSize, EConstantBufferType type);
	Texture* createTextureFromFile(wchar_t* filename);
	Texture* createTexture2D(unsigned int width, unsigned int height, const void* data, bool createRenderTarget, unsigned int multiSampleCount = 1, unsigned int multiSampleQuality = 0);
	SKYENGINEDLL void setPrimitiveTopology(PRIMITIVE_TOPOLOGY primitiveType);
	SKYENGINEDLL void setRepeatMaxAnisoSampler(sky::EShaderType shaderType, unsigned int slot);
	SKYENGINEDLL void setClampLinearSampler(sky::EShaderType shaderType, unsigned int slot);
	SKYENGINEDLL void setBackFaceCulling(bool cullBackFace = true);
	SKYENGINEDLL void setDepthState(sky::EDepthState depthState);
	SKYENGINEDLL void setBlendState(sky::EBlendState blendState);
	SKYENGINEDLL void setBackBufferAsRenderTarget();
	SKYENGINEDLL void draw(unsigned int vertexCount, unsigned int startVertexIndex = 0);
	SKYENGINEDLL void drawIndexed(unsigned int IndexCount, unsigned int StartIndexLocation, int  BaseVertexLocation);
	SKYENGINEDLL void drawIndexedInstanced(unsigned int indexCountPerInstance, unsigned int instanceCount, unsigned int startIndexLocation, int  baseVertexLocation, unsigned int startInstanceLocation);
private:
	IDeviceImpl* _pDeviceImpl;
};

