#pragma once
#include "IEntity.h"
#include "TerrainMesh.h"

namespace scene
{
	class TerrainEntity :
		public IEntity
	{
	public:
		TerrainEntity();
		~TerrainEntity();
		void update(const XMFLOAT4X4 &viewMatrix, const XMFLOAT4X4 &projectMatrix);
		void setRotation(float angleX, float angleY, float angleZ);
		void setPosition(float x, float y, float z);
		void setScale(float scaleX, float scaleY, float scaleZ);
		void setBehavior(Behavior* behavior);
		SKYENGINEDLL TerrainMesh* getMesh() const;
		void setMesh(TerrainMesh* mesh);
		CBPerObject *getTransform() const;
	private:
		CBPerObject *m_baseTransform;
		XMFLOAT4X4 m_worldMatrix;
		XMFLOAT4X4 m_rotationMatrix;
		XMFLOAT4X4 m_translateMatrix;
		XMFLOAT4X4 m_scaleMatrix;
		TerrainMesh* m_Mesh;
		Behavior* m_behavior;
	};
}

