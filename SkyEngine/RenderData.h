#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "InputLayout.h"
#include "Texture.h"
#include "constants.h"
#include "RenderPass.h"
#include <vector>

using namespace DirectX;
using namespace std;

class RenderData
{
public:
	RenderData()
		:pVertexBuffer(nullptr),
		pIndexBuffer(nullptr),
		pVertexShader(nullptr),
		pPixelShader(nullptr),
		pInputLayout(nullptr),
		m_ViewProj(nullptr),
		constanBufferCounter(0),
		texturesCounter(0),
		vertexBufferCounter(0)
	{
		_translate.x = 0;
		_translate.y = 0;
		_translate.z = 0;

		_scale.x = 1;
		_scale.y = 1;
		_scale.z = 1;

		_rotation.x = 0;
		_rotation.y = 0;
		_rotation.z = 0;

		memset(_constantBuffers, 0, sizeof(ConstantBuffer*) * MAX_CONSTANT_BUFFERS_COUNT);
		memset(_textures, 0, sizeof(Texture*) * MAX_SHADER_RESOURCES_COUNT);
	}

	~RenderData()
	{
		if (pIndexBuffer)
		{
			delete pIndexBuffer;
			pIndexBuffer = nullptr;
		}

		if (pVertexBuffer)
		{
			delete pVertexBuffer;
			pVertexBuffer = nullptr;
		}
	}
	void updateTransform()
	{
		XMMATRIX mScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);

		XMMATRIX rotMatrix = XMMatrixRotationX(_rotation.x) * XMMatrixRotationY(_rotation.y) * XMMatrixRotationZ(_rotation.z);

		XMMATRIX transMat = XMMatrixTranslation(_translate.x, _translate.y, _translate.z);

		XMMATRIX world = mScale * rotMatrix * transMat;

		XMMATRIX viewProj = XMLoadFloat4x4(m_ViewProj);

		XMStoreFloat4x4(&_WVP, XMMatrixTranspose(world * viewProj));
		XMStoreFloat4x4(&_world, XMMatrixTranspose(world));
	}
	XMFLOAT4X4 getGlobalTransform() const
	{
		return _WVP;
	}
	XMFLOAT4X4 getLocalTransform() const
	{
		return _world;
	}
	void addConstantBuffer(ConstantBuffer* aCbuffer)
	{
		assert(constanBufferCounter < MAX_CONSTANT_BUFFERS_COUNT);

		_constantBuffers[constanBufferCounter] = aCbuffer;

		constanBufferCounter++;
	}
	ConstantBuffer* getConstantBuffer(unsigned int index) const
	{
		assert(constanBufferCounter >= index);

		return _constantBuffers[index];
	}
	void setVertexBuffer(VertexBuffer* aVertexBuffer)
	{
		pVertexBuffer = aVertexBuffer;
	}
	VertexBuffer* getVertexBuffer() const
	{
		return pVertexBuffer;
	}
	void setIndexBuffer(IndexBuffer* aIndexBuffer)
	{
		pIndexBuffer = aIndexBuffer;
	}
	IndexBuffer* getIndexBuffer() const
	{
		return pIndexBuffer;
	}
	/*void setVertexShader(Shader* aVertexShader)
	{
		pVertexShader = aVertexShader;
	}
	Shader* getVertexShader() const
	{
		return pVertexShader;
	}*/
	/*void setPixelShader(Shader* aPixelShader)
	{
		pPixelShader = aPixelShader;
	}
	Shader* getPixelShader() const
	{
		return pPixelShader;
	}*/
	void setInputLayout(InputLayout* aInputLayout)
	{
		pInputLayout = aInputLayout;
	}
	InputLayout* getInputLayout() const
	{
		return pInputLayout;
	}
	void addTexture(Texture* aTexture)
	{
		assert(texturesCounter < MAX_SHADER_RESOURCES_COUNT);

		_textures[texturesCounter] = aTexture;

		texturesCounter++;
	}
	void setTexture(Texture* aTexture, unsigned int index)
	{
		assert(index < MAX_SHADER_RESOURCES_COUNT);

		_textures[index] = aTexture;
	}
	Texture* getTexture(unsigned int index) const
	{
		return _textures[index];
	}
	void setViewProj(XMFLOAT4X4 *aViewProj)
	{
		m_ViewProj = aViewProj;
	}
	XMFLOAT4X4 *getViewProj() const
	{
		return m_ViewProj;
	}
	void setTranslate(XMFLOAT3 &position)
	{
		_translate = position;
	}
	void setScale(XMFLOAT3 &scale)
	{
		_scale = scale;
	}
	void setRotation(XMFLOAT3 &rotation)
	{
		_rotation = rotation;
	}
	void addRenderPass(RenderPass* renderPass)
	{
		_renderPasses.push_back(renderPass);
	}
	RenderPass* getRenderPass(unsigned int index)
	{
		return _renderPasses[index];
	}
	unsigned int getRenderPassCount() const
	{
		return _renderPasses.size();
	}
	void dropRenderPass(unsigned int index)
	{
		if (_renderPasses[index])
		{
			delete _renderPasses[index];
			_renderPasses[index] = nullptr;
			_renderPasses.erase(_renderPasses.begin() + index);
		}
	}
	XMFLOAT4X4 getClipProjectionMatrix(XMFLOAT4& clip_plane)
	{
		XMMATRIX WorldToProjection = XMLoadFloat4x4(m_ViewProj);

		XMVECTOR clipPlan = XMLoadFloat4(&clip_plane);

		clipPlan = XMPlaneNormalize(clipPlan);

		WorldToProjection = XMMatrixInverse(nullptr, WorldToProjection);
		WorldToProjection = XMMatrixTranspose(WorldToProjection);

		XMVECTOR clipPlane = XMVectorSet(clip_plane.x, clip_plane.y, clip_plane.z, clip_plane.w);

		XMVECTOR projClipPlane = XMVector4Transform(clipPlane, WorldToProjection);

		XMMATRIX matClipProj = XMMatrixIdentity();

		if (projClipPlane.m128_f32[3] == 0)
		{
			return *m_ViewProj;
		}

		if (projClipPlane.m128_f32[3] > 0)
		{
			XMVECTOR clipPlane = XMVectorSet(-clip_plane.x, -clip_plane.y, -clip_plane.z, -clip_plane.w);

			projClipPlane = XMVector4Transform(clipPlane, WorldToProjection);
		}

		matClipProj.r[0].m128_f32[2] = projClipPlane.m128_f32[0];
		matClipProj.r[1].m128_f32[2] = projClipPlane.m128_f32[1];
		matClipProj.r[2].m128_f32[2] = projClipPlane.m128_f32[2];
		matClipProj.r[3].m128_f32[2] = projClipPlane.m128_f32[3];

		XMMATRIX projClipMatrix = XMLoadFloat4x4(m_ViewProj) * matClipProj;

		XMFLOAT4X4 _projClipMatrix;
		XMStoreFloat4x4(&_projClipMatrix, projClipMatrix);

		return _projClipMatrix;
	}
	XMFLOAT4X4 getMirrorViewMatrix(XMFLOAT4& plane)
	{
		XMMATRIX matrix_View = XMLoadFloat4x4(m_ViewProj);

		XMVECTOR _plane = XMLoadFloat4(&plane);
		XMMATRIX mirrorViewMartix = XMMatrixReflect(_plane) * matrix_View;

		XMFLOAT4X4 _MirrorViewMatrix;
		XMStoreFloat4x4(&_MirrorViewMatrix, mirrorViewMartix);

		return _MirrorViewMatrix;
	}
private:
	XMFLOAT4X4 _WVP;
	XMFLOAT4X4 _world;
	VertexBuffer *pVertexBuffer;
	IndexBuffer *pIndexBuffer;
	ConstantBuffer* _constantBuffers[MAX_CONSTANT_BUFFERS_COUNT];
	Shader *pVertexShader;
	Shader *pPixelShader;
	InputLayout *pInputLayout;
	Texture *_textures[MAX_SHADER_RESOURCES_COUNT];
	XMFLOAT4X4 *m_ViewProj;
	XMFLOAT3 _translate;
	XMFLOAT3 _scale;
	XMFLOAT3 _rotation;
	unsigned int constanBufferCounter;
	unsigned int texturesCounter;
	unsigned int vertexBufferCounter;

	vector<RenderPass*> _renderPasses;
};
