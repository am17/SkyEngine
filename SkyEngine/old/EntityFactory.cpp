#include "stdafx.h"
#include "EntityFactory.h"

namespace sky
{
	EntityFactory::EntityFactory(IDevice *m_device, ShaderManager* shaderManager)
		: m_device(m_device), 
		m_geometryCreator(nullptr),
		m_shaderManager(shaderManager)
	{
		m_geometryCreator = new GeometryCreator(shaderManager);
	}


	EntityFactory::~EntityFactory()
	{
		if (m_geometryCreator)
		{
			delete m_geometryCreator;
			m_geometryCreator = nullptr;
		}
	}

	Entity* EntityFactory::create() const
	{
		Entity *newEntity = new Entity();

		//IAPIBuffer *vertexBuffer = m_device->createVertexBuffer(vertex_buffer->getRawData(), vertex_buffer->getVerticesCount(), vertex_buffer->getStride());

		return newEntity;
	}

	TerrainEntity *EntityFactory::createTerrainEntity(const char* heightMapFile, int terrainHeight, int terrainWidth) const
	{
		TerrainEntity *newEntity = new TerrainEntity();

		TerrainMesh *mesh = m_geometryCreator->createTerrainMesh(heightMapFile, terrainHeight, terrainWidth);
		
		newEntity->setMesh(mesh);

		return newEntity;
	}

	Entity *EntityFactory::createSkydomeEntity() const
	{
		Entity *newEntity = new Entity();

		StaticMesh *mesh = m_geometryCreator->createSkydomeMesh();
		newEntity->setStaticMesh(mesh);

		return newEntity;
	}

	GeometryCreator* EntityFactory::getGeometryCreator() const
	{
		return m_geometryCreator;
	}
}
