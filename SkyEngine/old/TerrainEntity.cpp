#include "stdafx.h"
#include "TerrainEntity.h"

namespace scene
{
	TerrainEntity::TerrainEntity()
		:m_baseTransform(nullptr),
		m_Mesh(nullptr),
		m_behavior(nullptr)
	{
		XMStoreFloat4x4(&m_worldMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&m_rotationMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&m_translateMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&m_scaleMatrix, XMMatrixIdentity());

		m_baseTransform = new CBPerObject();
	}

	TerrainEntity::~TerrainEntity()
	{
		if (m_behavior)
		{
			delete m_behavior;
			m_behavior = nullptr;
		}

		if (m_baseTransform)
		{
			delete m_baseTransform;
			m_baseTransform = nullptr;
		}
	}

	void TerrainEntity::update(const XMFLOAT4X4 &pViewMatrix, const XMFLOAT4X4 &pProjectMatrix)
	{
		XMMATRIX viewMatrix = XMLoadFloat4x4(&pViewMatrix);
		XMMATRIX projectMatrix = XMLoadFloat4x4(&pProjectMatrix);
		XMMATRIX worldMatrix = XMLoadFloat4x4(&m_worldMatrix);

		XMMATRIX rotationMatrix = XMLoadFloat4x4(&m_rotationMatrix);
		XMMATRIX translateMatrix = XMLoadFloat4x4(&m_translateMatrix);
		XMMATRIX scaleMatrix = XMLoadFloat4x4(&m_scaleMatrix);

		worldMatrix = scaleMatrix * rotationMatrix * translateMatrix;

		XMMATRIX WVP = worldMatrix * viewMatrix * projectMatrix;

		XMStoreFloat4x4(&m_baseTransform->WVP, XMMatrixTranspose(WVP));
		XMStoreFloat4x4(&m_baseTransform->world, XMMatrixTranspose(worldMatrix));

		XMStoreFloat4x4(&m_worldMatrix, worldMatrix);
	}
	void TerrainEntity::setRotation(float angleX, float angleY, float angleZ)
	{
		XMMATRIX rotMat = XMMatrixRotationX(angleX) * XMMatrixRotationY(angleY) * XMMatrixRotationZ(angleZ);

		XMStoreFloat4x4(&m_rotationMatrix, rotMat);
	}
	void TerrainEntity::setPosition(float x, float y, float z)
	{
		XMMATRIX transMat = XMMatrixTranslation(x, y, z);

		XMStoreFloat4x4(&m_translateMatrix, transMat);
	}
	void TerrainEntity::setScale(float scaleX, float scaleY, float scaleZ)
	{
		XMMATRIX scaleMat = XMMatrixScaling(scaleX, scaleY, scaleZ);

		XMStoreFloat4x4(&m_scaleMatrix, scaleMat);
	}
	void TerrainEntity::setBehavior(Behavior* behavior)
	{

	}

	TerrainMesh* TerrainEntity::getMesh() const
	{
		return m_Mesh;
	}

	void TerrainEntity::setMesh(TerrainMesh* mesh)
	{
		m_Mesh = mesh;
	}

	CBPerObject* TerrainEntity::getTransform() const
	{
		return m_baseTransform;
	}
}
