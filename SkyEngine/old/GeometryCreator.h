#pragma once
#include <set>
#include "StaticMesh.h"
#include "ColorMesh.h"
#include "ShaderManager.h"
#include "GeometricPrimitive.h"
#include "TerrainMesh.h"
#include "ColorMeshRenderData.h"

using namespace DirectX;
using namespace std;

namespace sky
{
	enum class EBASIC_MESHES
	{
		MESH_TRIANGLE		= 0,
		MESH_CUBE			= 1,
		MESH_TEAPOT			= 2,
		MESH_SPHERE			= 3,
		MESH_BOX			= 4,
		MESH_GEOSPHERE		= 5,
		MESH_CYLINDER		= 6,
		MESH_CONE			= 7,
		MESH_TORUS			= 8,
		MESH_TETRAHEDRON	= 9,
		MESH_OCTAHEDRON		= 10,
		MESH_DODECAHEDRON	= 11,
		MESH_ICOSAHEDRON	= 12,
		MESH_PLANE = 13
	};

	struct Triangle
	{
		unsigned short  index[3];
	};

	class GeometryCreator
	{
	public:
		GeometryCreator(ShaderManager* shaderManager);
		~GeometryCreator();
		TerrainMesh *createTerrainMesh(const char* heightMapFile, int terrainHeight, int terrainWidth);
		StaticMesh *createSkydomeMesh();
		ColorMesh *createColorMesh(vector<ColorVertex>& verticess, vector<unsigned int>& indices);
		template<EBASIC_MESHES basicMesh>
		StaticMesh *createMesh()
		{
			vector<VertexPositionNormalTexture> verticess;
			vector<uint16_t> indices;

			switch (basicMesh)
			{
			case sky::EBASIC_MESHES::MESH_CUBE:
				GeometricPrimitive::CreateCube(verticess, indices, 1.0f, false);
				break;
			case sky::EBASIC_MESHES::MESH_TEAPOT:
				GeometricPrimitive::CreateTeapot(verticess, indices, 1.0f, 8, false);
				break;
			case sky::EBASIC_MESHES::MESH_SPHERE:
				GeometricPrimitive::CreateSphere(verticess, indices, 1, 16, false);
				break;
			case sky::EBASIC_MESHES::MESH_BOX:
				GeometricPrimitive::CreateBox(verticess, indices, XMFLOAT3(1.0f, 1.0f, 1.0f), false);
				break;
			case sky::EBASIC_MESHES::MESH_GEOSPHERE:
				GeometricPrimitive::CreateGeoSphere(verticess, indices, 1.0f, 3, false);
				break;
			case sky::EBASIC_MESHES::MESH_CYLINDER:
				GeometricPrimitive::CreateCylinder(verticess, indices, 1.0f, 1.0f, 32, false);
				break;
			case sky::EBASIC_MESHES::MESH_CONE:
				GeometricPrimitive::CreateCone(verticess, indices, 1.0f, 1.0f, 32, false);
				break;
			case sky::EBASIC_MESHES::MESH_TORUS:
				GeometricPrimitive::CreateTorus(verticess, indices, 1.0f, 0.333f, 32, false);
				break;
			case sky::EBASIC_MESHES::MESH_TETRAHEDRON:
				GeometricPrimitive::CreateTetrahedron(verticess, indices, 1.0f, false);
				break;
			case sky::EBASIC_MESHES::MESH_OCTAHEDRON:
				GeometricPrimitive::CreateOctahedron(verticess, indices, 1.0f, false);
				break;
			case sky::EBASIC_MESHES::MESH_DODECAHEDRON:
				GeometricPrimitive::CreateDodecahedron(verticess, indices, 1.0f, false);
				break;
			case sky::EBASIC_MESHES::MESH_ICOSAHEDRON:
				GeometricPrimitive::CreateIcosahedron(verticess, indices, 1.0f, false);
				break;
			case EBASIC_MESHES::MESH_PLANE:
				verticess.push_back(VertexPositionNormalTexture(XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)));
				verticess.push_back(VertexPositionNormalTexture(XMFLOAT3(1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)));
				verticess.push_back(VertexPositionNormalTexture(XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)));
				verticess.push_back(VertexPositionNormalTexture(XMFLOAT3(-1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)));
				indices.push_back(0);
				indices.push_back(2);
				indices.push_back(1);
				indices.push_back(0);
				indices.push_back(3);
				indices.push_back(2);
				break;
			case EBASIC_MESHES::MESH_TRIANGLE:
				verticess.push_back(VertexPositionNormalTexture(XMFLOAT3(-0.5, -0.5, -0.5), XMFLOAT3(0, 0, 1), XMFLOAT2(1, 1)));
				verticess.push_back(VertexPositionNormalTexture(XMFLOAT3(0.5, -0.5, 0.5), XMFLOAT3(0, 0, 1), XMFLOAT2(0, 1)));
				verticess.push_back(VertexPositionNormalTexture(XMFLOAT3(0.5, 0.5, 0.5), XMFLOAT3(0, 0, 1), XMFLOAT2(0, 0)));
				indices.push_back(0);
				indices.push_back(1);
				indices.push_back(2);
			default:
				break;
			}

			vector<StaticMeshVertex> formatedVertices(verticess.size());
			vector<unsigned int> formatedindices(indices.size());

			for (size_t i = 0; i < formatedVertices.size(); i++)
			{
				formatedVertices[i].position = verticess[i].position;
				formatedVertices[i].normal = verticess[i].normal;
				formatedVertices[i].UVs = verticess[i].textureCoordinate;
			}

			for (size_t i = 0; i < formatedindices.size(); i++)
			{
				formatedindices[i] = static_cast<unsigned int>(indices[i]);
			}

			return initStaticMesh(formatedVertices, formatedindices);
		}
	private:
		StaticMesh *initStaticMesh(vector<StaticMeshVertex>& verticess, vector<unsigned int>& indices, bool cullBackface = true, bool zBufferEnabled = true);
		StaticMesh *initStaticMesh(StaticMeshRenderData *renderData);
		ColorMesh *initColorMesh(vector<ColorVertex>& verticess, vector<unsigned int>& indices);
		TerrainMesh *initMeshFromRaw(const char* heightMapFile, int terrainHeight, int terrainWidth) const;
		set<StaticMesh*> m_meshes;
		set<TerrainMesh*> m_terrainMeshes;
		set<ColorMesh*> m_colorMeshes;
		ShaderManager* m_shaderManager;
	};
}

