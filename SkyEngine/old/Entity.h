#pragma once
#include "IEntity.h"
#include "StaticMesh.h"
#include "common.h"

using namespace sky;

namespace scene
{
	class Entity :
		public IEntity
	{
	public:
		Entity();
		~Entity();
		void update(const XMFLOAT4X4 &viewMatrix, const XMFLOAT4X4 &projectMatrix) override;
		void Entity::setRotation(float angleX, float angleY, float angleZ) override;
		void setPosition(float x, float y, float z) override;
		void setScale(float ScaleX, float ScaleY, float ScaleZ) override;
		SKYENGINEDLL void setBehavior(Behavior* behavior) override;
		SKYENGINEDLL StaticMesh* getMesh() const;
		void setStaticMesh(StaticMesh* mesh);
		CBPerObject *getTransform() const;
	private:
		CBPerObject *m_baseTransform;
		XMFLOAT4X4 m_worldMatrix;
		XMFLOAT4X4 m_rotationMatrix;
		XMFLOAT4X4 m_translateMatrix;
		XMFLOAT4X4 m_scaleMatrix;
		StaticMesh* m_staticMesh;
		Behavior* m_behavior;
	};
}

