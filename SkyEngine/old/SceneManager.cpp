#include "stdafx.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Entity.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace scene
{
	SceneManager::SceneManager()
		:m_device(nullptr),
		m_camera(nullptr),
		m_projectionMatrix(nullptr),
		m_shaderManager(nullptr), 
		m_entityFactory(nullptr),
		testEntity(nullptr),
		mesh(nullptr)
	{
	}

	SceneManager::~SceneManager()
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

		if (m_entityFactory)
		{
			delete m_entityFactory;
			m_entityFactory = nullptr;
		}
		
		if (m_camera)
		{
			delete m_camera;
			m_camera = nullptr;
		}

		if (m_projectionMatrix)
		{
			delete m_projectionMatrix;
			m_projectionMatrix = nullptr;
		}

		if (m_shaderManager)
		{
			delete m_shaderManager;
			m_shaderManager = nullptr;
		}
	}

	bool SceneManager::init(IDevice *device)
	{
		if (!device)
		{
			return false;
		}

		m_device = device;

		m_shaderManager = new ShaderManager();

		m_entityFactory = new EntityFactory(device);

		m_camera = new Camera();
		if (!m_camera)
		{
			return false;
		}

		m_camera->setPosition(0.0f, 2.0f, -10.0f);

		float fieldOfView = (float)XM_PI / 4.0f;
		float screenAspect = m_device->getScreenAspectRatio();

		XMMATRIX projectionMat = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 1000.0f);

		m_projectionMatrix = new XMFLOAT4X4();

		XMStoreFloat4x4(m_projectionMatrix, projectionMat);

		return true;
	}
	bool SceneManager::drawScene() const
	{
		m_camera->update();

		if (testEntity)
		{
			static float rotation = 0.0f;

			rotation += static_cast<float>(XM_PI) * 0.005f;
			if (rotation > 360.0f)
			{
				rotation -= 360.0f;
			}

			testEntity->setRotation(0, rotation, 0);

			testEntity->update(m_camera->getViewMatrix(), *m_projectionMatrix);

			IRenderTemplate *renderTemplate = testEntity->getRenderTemplate();

			m_device->draw(*renderTemplate);
		}

		return true;
	}

	bool SceneManager::addSceneNode()
	{
		testEntity = m_entityFactory->create();

		mesh = new StaticMesh();

		mesh->initResources();

		testEntity->setStaticMesh(mesh);
		//testEntity->getStaticMeshComponent()->setMaterial(0, "BasicShapeMaterial");

		StaticMeshVertexBuffer *vertex_buffer = mesh->getRenderData()->getResources().getVertexBuffer();

		IAPIBuffer *vertexBuffer = m_device->createVertexBuffer(vertex_buffer->getRawData(), vertex_buffer->getNumVertices(), vertex_buffer->getStride());

		vertex_buffer->setAPIVertexBuffer(vertexBuffer);

		testEntity->getRenderTemplate()->getIAStageData()->addVertexBuffers(vertexBuffer);

		StaticIndexBuffer *index_buffer = mesh->getRenderData()->getResources().getIndexBuffer();

		IAPIBuffer *indexBuffer = m_device->createIndexBuffer(index_buffer->getRawData(), index_buffer->getNumIndeces(), index_buffer->getStride());

		testEntity->getRenderTemplate()->getIAStageData()->setIndexBuffer(indexBuffer);

		CompileResult compRes = m_shaderManager->compileFromFile(L"colorVS.hlsl", "ColorVertexShader", "vs_4_0");

		if (compRes.isSucceeded)
		{
			IShader *vertexShader = m_device->createVertexShader(compRes.pByteCode, compRes.byteCodeLength);

			VertexInputLayout* layout = m_device->getInputLayout(vertex_buffer->getVertexDeclaration(), 6, compRes.pByteCode, compRes.byteCodeLength);

			IAPIBuffer *constantBuffer = m_device->createConstantBuffer(sizeof(MatrixBufferType));

			if (indexBuffer == nullptr || layout == nullptr || constantBuffer == nullptr)
			{
				return false;
			}

			vertexShader->addConstantBuffer(constantBuffer);

			testEntity->getRenderTemplate()->getVSStageData()->setVertexShader(vertexShader);
			testEntity->getRenderTemplate()->getIAStageData()->setInputLayout(layout);
		}

		compRes = m_shaderManager->compileFromFile(L"colorPS.hlsl", "ColorPixelShader", "ps_4_0");
		if (compRes.isSucceeded)
		{
			IShader *pixelShader = m_device->createPixelShader(compRes.pByteCode, compRes.byteCodeLength);

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