#pragma once
#include "IDevice.h"
#include "ICamera.h"
#include "VertexFormat.h"
#include "ShaderManager.h"
#include "RenderTemplate.h"
#include "IEntity.h"
#include "StaticMesh.h"
#include "Entity.h"
#include "EntityFactory.h"

using namespace sky;
using namespace render;

namespace scene
{
	class SceneManager
	{
	public:
		SKYENGINEDLL SceneManager();
		SKYENGINEDLL ~SceneManager();
		SKYENGINEDLL bool init(IDevice *device);
		SKYENGINEDLL bool drawScene() const;
		SKYENGINEDLL bool addSceneNode();
		//{
		//	Vertex1P1C* vertices = new Vertex1P1C[3];

		//	unsigned long* indices = new unsigned long[3];

		//	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
		//	vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

		//	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
		//	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

		//	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
		//	vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

		//	indices[0] = 0;
		//	indices[1] = 1;
		//	indices[2] = 2;

		//	Mesh<Vertex1P1C> *mesh = new Mesh<Vertex1P1C>(vertices, 3, indices, 3);

		//	return mesh;
		//}
	private:
		IDevice *m_device;
		ICamera *m_camera;
		XMFLOAT4X4 *m_projectionMatrix;
		ShaderManager *m_shaderManager;
		EntityFactory *m_entityFactory;

		Entity *testEntity;
		StaticMesh *mesh;
	};
}

