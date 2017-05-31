#include "stdafx.h"
#include "Scene.h"
#include "Camera.h"
#include "Entity.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace scene
{
	Scene::Scene()
		:m_render(nullptr),
		m_camera(nullptr), 
		entityFactory(nullptr),
		testEntity(nullptr),
		mesh(nullptr)
	{
	}

	Scene::~Scene()
	{
		if (mesh)
		{
			delete mesh;
			mesh = nullptr;
		}

		if (testEntity)
		{
			delete testEntity;
			testEntity = nullptr;
		}

		if (entityFactory)
		{
			delete entityFactory;
			entityFactory = nullptr;
		}
		
		if (m_camera)
		{
			delete m_camera;
			m_camera = nullptr;
		}
	}

	bool Scene::init(Render* render)
	{
		m_render = render;

		entityFactory = new EntityFactory(m_render->getDevice());

		m_camera = new Camera();

		m_camera->setPosition(0.0f, 2.0f, -10.0f);

		return true;
	}
	bool Scene::draw() const
	{
		m_camera->update();

		if (testEntity)
		{
			testEntity->update(m_camera->getViewMatrix(), m_render->getProjectMatrix());

			IRenderTemplate *renderTemplate = testEntity->getRenderTemplate();

			m_render->bindRenderResources(*renderTemplate);
		}

		return true;
	}

	bool Scene::addSceneNode()
	{
		testEntity = new Entity();

		mesh = new StaticMesh();

		mesh->initResources();

		testEntity->setStaticMesh(mesh);

		StaticMeshVertexBuffer *vertex_buffer = mesh->getRenderData()->getResources().getVertexBuffer();

		IAPIBuffer *vertexBuffer = m_render->getDevice()->createVertexBuffer(vertex_buffer->getRawData(), vertex_buffer->getNumVertices(), vertex_buffer->getStride());

		///

		vertex_buffer->setAPIVertexBuffer(vertexBuffer);

		testEntity->getRenderTemplate()->getIAStageData()->addVertexBuffers(vertexBuffer);

		StaticIndexBuffer *index_buffer = mesh->getRenderData()->getResources().getIndexBuffer();

		IAPIBuffer *indexBuffer = m_render->getDevice()->createIndexBuffer(index_buffer->getRawData(), index_buffer->getNumIndeces(), index_buffer->getStride());

		testEntity->getRenderTemplate()->getIAStageData()->setIndexBuffer(indexBuffer);

		CompileResult compRes = m_render->getShaderManager()->compileFromFile(L"colorVS.hlsl", "ColorVertexShader", "vs_4_0");

		if (compRes.isSucceeded)
		{
			IShader *vertexShader = m_render->getDevice()->createVertexShader(compRes.pByteCode, compRes.byteCodeLength);

			VertexInputLayout* layout = m_render->getDevice()->getInputLayout(vertex_buffer->getVertexDeclaration(), 6, compRes.pByteCode, compRes.byteCodeLength);

			IAPIBuffer *constantBuffer = m_render->getDevice()->createConstantBuffer(sizeof(MatrixBufferType));

			if (indexBuffer == nullptr || layout == nullptr || constantBuffer == nullptr)
			{
				return false;
			}

			vertexShader->addConstantBuffer(constantBuffer);

			testEntity->getRenderTemplate()->getVSStageData()->setVertexShader(vertexShader);
			testEntity->getRenderTemplate()->getIAStageData()->setInputLayout(layout);
		}

		compRes = m_render->getShaderManager()->compileFromFile(L"colorPS.hlsl", "ColorPixelShader", "ps_4_0");
		if (compRes.isSucceeded)
		{
			IShader *pixelShader = m_render->getDevice()->createPixelShader(compRes.pByteCode, compRes.byteCodeLength);

			if (pixelShader == nullptr)
			{
				return false;
			}

			testEntity->getRenderTemplate()->getPSStageData()->setPixelShader(pixelShader);
		}

		testEntity->getRenderTemplate()->setReady(true);

		return true;
	}
}
