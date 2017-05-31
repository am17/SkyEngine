#include "stdafx.h"
#include "Device.h"
#include "D3D11DeviceImp.h"


Device::Device()
	:_pDeviceImpl(nullptr)
{
}


Device::~Device()
{
	if (_pDeviceImpl)
	{
		delete _pDeviceImpl;
		_pDeviceImpl = nullptr;
	}
}

bool Device::init(HWND m_hwnd, int screenWidth, int screenHeight, bool fullscreen)
{
	_pDeviceImpl = new D3D11DeviceImpl();

	return _pDeviceImpl->init(m_hwnd, screenWidth, screenHeight, fullscreen);
}

void Device::clearBackBuffer()
{
	_pDeviceImpl->clearBackBuffer();
}
void Device::present()
{
	_pDeviceImpl->present();
}

VertexBuffer* Device::createVertexBuffer(const void *pData, unsigned int elementsCount, unsigned int stride)
{
	VertexBuffer *vertexBuffer = _pDeviceImpl->createVertexBuffer(pData, elementsCount, stride);

	return vertexBuffer;
}

IndexBuffer* Device::createIndexBuffer(const void *pData, size_t elementsCount, unsigned int stride, INDEX_BUFFER_FORMAT format)
{
	IndexBuffer* indexBuffer = _pDeviceImpl->createIndexBuffer(pData, elementsCount, stride, format);

	return indexBuffer;
}

Shader* Device::createVertexShader(const void* pByteCode, size_t ByteCodeLength)
{
	Shader* vs = _pDeviceImpl->createVertexShader(pByteCode, ByteCodeLength);

	return vs;
}
Shader* Device::createPixelShader(const void* pByteCode, size_t ByteCodeLength)
{
	Shader* ps = _pDeviceImpl->createPixelShader(pByteCode, ByteCodeLength);

	return ps;
}
InputLayout* Device::createInputLayout(VertexLayoutDesc *layoutDesc, const unsigned int layoutDescLength, const void* pByteCode, size_t ByteCodeLength)
{
	InputLayout* layout = _pDeviceImpl->createInputLayout(layoutDesc, layoutDescLength, pByteCode, ByteCodeLength);

	return layout;
}
ConstantBuffer* Device::createConstantBuffer(unsigned int bufferSize, EConstantBufferType type)
{
	ConstantBuffer* cb = _pDeviceImpl->createConstantBuffer(bufferSize, type);

	return cb;
}

Texture* Device::createTextureFromFile(wchar_t* filename)
{
	Texture *texture = _pDeviceImpl->createTextureFromFile(filename);

	return texture;
}

Texture* Device::createFresnelMap()
{
	Texture *texture = _pDeviceImpl->createFresnelMap();

	return texture;
}

void Device::clearRenderTarget(ERenderTarget renderTarget)
{
	_pDeviceImpl->clearRenderTarget(renderTarget);
}

Texture* Device::getDepthMap()
{
	return _pDeviceImpl->getDepthMap();
}

void Device::setPrimitiveTopology(PRIMITIVE_TOPOLOGY primitiveType)
{
	_pDeviceImpl->setPrimitiveTopology(primitiveType);
}

void Device::setRepeatMaxAnisoSampler(sky::EShaderType shaderType, unsigned int slot)
{
	_pDeviceImpl->setRepeatMaxAnisoSampler(shaderType, slot);
}
void Device::setClampLinearSampler(sky::EShaderType shaderType, unsigned int slot)
{
	_pDeviceImpl->setClampLinearSampler(shaderType, slot);
}

void Device::setBackFaceCulling(bool cullBackFace)
{
	_pDeviceImpl->setBackFaceCulling(cullBackFace);
}

void Device::setDepthState(sky::EDepthState depthState)
{
	_pDeviceImpl->setDepthState(depthState);
}

void Device::setBlendState(sky::EBlendState blendState)
{
	_pDeviceImpl->setBlendState(blendState);
}

void Device::setRenderTarget(ERenderTarget renderTarget)
{
	_pDeviceImpl->setRenderTarget(renderTarget);
}

void Device::drawIndexed(unsigned int IndexCount, unsigned int StartIndexLocation, int  BaseVertexLocation)
{
	_pDeviceImpl->drawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

void Device::drawIndexedInstanced(unsigned int indexCountPerInstance, unsigned int instanceCount, unsigned int startIndexLocation, int  baseVertexLocation, unsigned int startInstanceLocation)
{
	_pDeviceImpl->drawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}
