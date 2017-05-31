#pragma once
#include "IDevice.h"
#include "Entity.h"
#include "GeometryCreator.h"
#include "ShaderManager.h"
#include "TerrainEntity.h"

using namespace scene;

namespace sky
{
	enum class EBASIC_ENTITY
	{
		ENTITY_TRIANGLE = 0,
		ENTITY_CUBE = 1,
		ENTITY_TEAPOT = 2,
		ENTITY_SPHERE = 3,
		ENTITY_BOX = 4,
		ENTITY_GEOSPHERE = 5,
		ENTITY_CYLINDER = 6,
		ENTITY_CONE = 7,
		ENTITY_TORUS = 8,
		ENTITY_TETRAHEDRON = 9,
		ENTITY_OCTAHEDRON = 10,
		ENTITY_DODECAHEDRON = 11,
		ENTITY_ICOSAHEDRON = 12,
		ENTITY_PLANE = 13
	};

	class EntityFactory
	{
	public:
		EntityFactory(IDevice *m_device, ShaderManager* shaderManager);
		~EntityFactory();
		Entity *create() const;
		TerrainEntity *createTerrainEntity(const char* heightMapFile, int terrainHeight, int terrainWidth) const;
		Entity *createSkydomeEntity() const;
		GeometryCreator *getGeometryCreator() const;
		template<EBASIC_ENTITY entityType>
		Entity *create()
		{
			Entity *newEntity = new Entity();

			StaticMesh *mesh = nullptr;

			switch (entityType)
			{
			case sky::EBASIC_ENTITY::ENTITY_CUBE:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_CUBE>();
				break;
			case sky::EBASIC_ENTITY::ENTITY_TEAPOT:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_TEAPOT>();
				break;
			case sky::EBASIC_ENTITY::ENTITY_SPHERE:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_SPHERE>();
				break;
			case sky::EBASIC_ENTITY::ENTITY_BOX:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_BOX>();
				break;
			case sky::EBASIC_ENTITY::ENTITY_GEOSPHERE:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_GEOSPHERE>();
				break;
			case sky::EBASIC_ENTITY::ENTITY_CYLINDER:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_CYLINDER>();
				break;
			case sky::EBASIC_ENTITY::ENTITY_CONE:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_CONE>();
				break;
			case sky::EBASIC_ENTITY::ENTITY_TORUS:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_TORUS>();
				break;
			case sky::EBASIC_ENTITY::ENTITY_TETRAHEDRON:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_TETRAHEDRON>();
				break;
			case sky::EBASIC_ENTITY::ENTITY_OCTAHEDRON:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_OCTAHEDRON>();
				break;
			case sky::EBASIC_ENTITY::ENTITY_DODECAHEDRON:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_DODECAHEDRON>();
				break;
			case sky::EBASIC_ENTITY::ENTITY_ICOSAHEDRON:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_ICOSAHEDRON>();
				break;
			case EBASIC_ENTITY::ENTITY_PLANE:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_PLANE>();
				break;
			case EBASIC_ENTITY::ENTITY_TRIANGLE:
				mesh = m_geometryCreator->createMesh<EBASIC_MESHES::MESH_TRIANGLE>();
			default:
				break;
			}

			assert(mesh != nullptr);

			newEntity->setStaticMesh(mesh);

			return newEntity;
		}
	private:
		IDevice *m_device;
		GeometryCreator *m_geometryCreator;
		ShaderManager* m_shaderManager;
	};
}

