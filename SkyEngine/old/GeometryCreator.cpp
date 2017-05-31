#include "stdafx.h"
#include "GeometryCreator.h"
#include "DirectXMesh.h"

using namespace DirectX;

namespace sky
{
	GeometryCreator::GeometryCreator(ShaderManager* shaderManager)
		:m_shaderManager(shaderManager)
	{
	}

	GeometryCreator::~GeometryCreator()
	{
		for (set<StaticMesh*>::const_iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
		{
			StaticMesh *mesh = (*it);
			if (mesh)
			{
				delete mesh;
				mesh = nullptr;
			}
		}

		for (set<TerrainMesh*>::const_iterator it = m_terrainMeshes.begin(); it != m_terrainMeshes.end(); ++it)
		{
			TerrainMesh *mesh = (*it);
			if (mesh)
			{
				delete mesh;
				mesh = nullptr;
			}
		}

		for (set<ColorMesh*>::const_iterator it = m_colorMeshes.begin(); it != m_colorMeshes.end(); ++it)
		{
			ColorMesh *mesh = (*it);
			if (mesh)
			{
				delete mesh;
				mesh = nullptr;
			}
		}
	}

	TerrainMesh *GeometryCreator::createTerrainMesh(const char* heightMapFile, int terrainHeight, int terrainWidth)
	{
		TerrainMesh *mesh = initMeshFromRaw(heightMapFile, terrainHeight, terrainWidth);

		IMaterial *baseMaterial = new Material();
		baseMaterial->setTextureCount(2);

		mesh->setMaterial(0, baseMaterial);

		m_terrainMeshes.insert(mesh);

		size_t cellCount = mesh->getCellsCount();
		for (size_t i = 0; i < cellCount; i++)
		{
			TerrainCell* cell = mesh->getCell(i);

			BoundingBox aabb = *cell->getBoundingBox();

			vector<ColorVertex> vertices(8);
			vector<unsigned int> indices(24);

			XMFLOAT3 corners[8];

			aabb.GetCorners(corners);

			for (size_t j = 0; j < 8; j++)
			{
				vertices[j] = ColorVertex(corners[j], XMFLOAT4(0, 1, 1, 1));
			}

			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 1;
			indices[3] = 2;
			indices[4] = 2;
			indices[5] = 3;
			indices[6] = 3;
			indices[7] = 0;

			indices[8] = 4;
			indices[9] = 5;
			indices[10] = 5;
			indices[11] = 6;
			indices[12] = 6;
			indices[13] = 7;
			indices[14] = 7;
			indices[15] = 4;

			indices[16] = 0;
			indices[17] = 4;
			indices[18] = 1;
			indices[19] = 5;
			indices[20] = 2;
			indices[21] = 6;
			indices[22] = 3;
			indices[23] = 7;

			ColorMesh *debugMesh = createColorMesh(vertices, indices);

			cell->setDebugMesh(debugMesh);
		}

		return mesh;
	}

	StaticMesh *GeometryCreator::createSkydomeMesh()
	{
		vector<VertexPositionNormalTexture> verticess;
		vector<uint16_t> indices;

		GeometricPrimitive::CreateGeoSphere(verticess, indices, 5.0f, 2, false);

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

		StaticMesh *mesh = initStaticMesh(formatedVertices, formatedindices, false, false);

		mesh->getMaterial(0)->setVertexShader(EVertexShader::VS_SKYBOX);
		mesh->getMaterial(0)->setPixelShader(EPixelShader::PS_SKYBOX);

		m_meshes.insert(mesh);

		return mesh;
	}

	ColorMesh* GeometryCreator::createColorMesh(vector<ColorVertex>& verticess, vector<unsigned int>& indices)
	{
		return initColorMesh(verticess, indices);
	}

	StaticMesh *GeometryCreator::initStaticMesh(vector<StaticMeshVertex>& verticess, vector<unsigned int>& indices, bool cullBackface, bool zBufferEnabled)
	{
		const size_t posCount = verticess.size();

		XMFLOAT3 *positions = new XMFLOAT3[posCount];
		XMFLOAT3 *normals = new XMFLOAT3[posCount];
		XMFLOAT2 *textcoods = new XMFLOAT2[posCount];
		XMFLOAT3 *tangents = new XMFLOAT3[posCount];
		XMFLOAT3 *binormals = new XMFLOAT3[posCount];

		for (size_t i = 0; i < verticess.size(); i++)
		{
			positions[i] = XMFLOAT3(verticess[i].position.x, verticess[i].position.y, verticess[i].position.z);
			normals[i] = XMFLOAT3(verticess[i].normal.x, verticess[i].normal.y, verticess[i].normal.z);
			textcoods[i] = verticess[i].UVs;
		}

		size_t numfaces = verticess.size() / 3;

		HRESULT res = ComputeTangentFrame(indices.data(), numfaces, positions, normals, textcoods, posCount, tangents, binormals);

		for (size_t i = 0; i < posCount; i++)
		{
			verticess[i].normal = normals[i];
			verticess[i].tangent = tangents[i];
		}

		if (positions)
		{
			delete positions;
			positions = nullptr;
		}

		if (normals)
		{
			delete normals;
			normals = nullptr;
		}

		if (textcoods)
		{
			delete textcoods;
			textcoods = nullptr;
		}

		if (tangents)
		{
			delete tangents;
			tangents = nullptr;
		}

		if (binormals)
		{
			delete binormals;
			binormals = nullptr;
		}

		StaticMeshVertexBuffer *VertexBuffer = new StaticMeshVertexBuffer();
		VertexBuffer->init(verticess);

		StaticIndexBuffer *indices_buffer = new StaticIndexBuffer();
		indices_buffer->init(indices);

		StaticMeshResources *meshResources = new StaticMeshResources(VertexBuffer, indices_buffer);

		StaticMesh *mesh = new StaticMesh();

		IVertexShader *vertexShader = m_shaderManager->getVertexShader(EVertexShader::VS_BASE);

		IPixelShader *pixelShader = m_shaderManager->getPixelShader(EPixelShader::PS_BASE);

		mesh->getRenderData()->setBackfaceCulling(cullBackface);

		mesh->getRenderData()->setZBufferEnabled(zBufferEnabled);

		mesh->getRenderData()->setResources(meshResources, vertexShader, pixelShader);

		mesh->setMaterialsCount(1);

		IMaterial *baseMaterial = new Material();
		baseMaterial->setTextureCount(pixelShader->getShaderResourceCount());

		mesh->setMaterial(0, baseMaterial);

		m_meshes.insert(mesh);

		return mesh;
	}

	StaticMesh *GeometryCreator::initStaticMesh(StaticMeshRenderData *renderData)
	{
		assert(renderData != nullptr);

		StaticMesh *mesh = new StaticMesh(renderData);

		mesh->setMaterialsCount(1);

		IPixelShader *pixelShader = renderData->getPipelineState()->getPSStageData()->getPixelShader();

		IMaterial *baseMaterial = new Material();
		baseMaterial->setTextureCount(pixelShader->getShaderResourceCount());

		mesh->setMaterial(0, baseMaterial);

		m_meshes.insert(mesh);

		return mesh;
	}

	ColorMesh * GeometryCreator::initColorMesh(vector<ColorVertex>& verticess, vector<unsigned int>& indices)
	{
		ColorVertexBuffer *VertexBuffer = new ColorVertexBuffer();
		VertexBuffer->init(verticess);

		StaticIndexBuffer *indices_buffer = new StaticIndexBuffer();
		indices_buffer->init(indices);

		ColorMeshResources *meshResources = new ColorMeshResources(VertexBuffer, indices_buffer);

		ColorMesh *mesh = new ColorMesh();

		IVertexShader *vertexShader = m_shaderManager->getVertexShader(EVertexShader::VS_COLOR);

		IPixelShader *pixelShader = m_shaderManager->getPixelShader(EPixelShader::PS_COLOR);

		mesh->getRenderData()->setResources(meshResources, vertexShader, pixelShader);

		m_colorMeshes.insert(mesh);

		return mesh;
	}

	TerrainMesh* GeometryCreator::initMeshFromRaw(const char* heightMapFile, int terrainHeight, int terrainWidth) const
	{
		string filename(heightMapFile);

		size_t extP = filename.rfind('.', filename.length());
		string ext = filename.substr(extP + 1, filename.length() - extP);

		assert(ext == "r16");

		TerrainMesh* mesh = new TerrainMesh(terrainHeight, terrainWidth);

		SHeightMapPoint *m_heightMap = mesh->loadHeightMapsFromRaw(heightMapFile);

		vector<StaticMeshVertex> vertices;
		vector<unsigned int> indices;

		int cols = terrainHeight;
		int rows = terrainWidth;

		size_t NumVertices = rows * cols;
		size_t NumFaces = (rows - 1)*(cols - 1) * 2;

		vertices.reserve(NumVertices);
		vertices.resize(NumVertices);

		//indices.reserve(NumFaces * 3);
		//indices.resize(NumFaces * 3);

		for (DWORD i = 0; i < rows; ++i)
		{
			for (DWORD j = 0; j < cols; ++j)
			{
				int index = (rows * j) + i;

				float x = i;
				float y = m_heightMap[index].y;
				float z = j;

				vertices[i*cols + j].position = XMFLOAT3(x, y, z);
				vertices[i*cols + j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				vertices[i*cols + j].color = XMFLOAT4(m_heightMap[index].r, m_heightMap[index].g, m_heightMap[index].b, 1);
			}
		}

		int k = 0;
		int texUIndex = 0;
		int texVIndex = 0;
		for (DWORD i = 0; i < rows - 1; i++)
		{
			for (DWORD j = 0; j < cols - 1; j++)
			{
				vertices[i*cols + j].UVs = XMFLOAT2(texUIndex + 0.0f, texVIndex + 1.0f);

				vertices[i*cols + j + 1].UVs = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

				vertices[(i + 1)*cols + j].UVs = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);

				vertices[(i + 1)*cols + j].UVs = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);

				vertices[i*cols + j + 1].UVs = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

				vertices[(i + 1)*cols + j + 1].UVs = XMFLOAT2(texUIndex + 1.0f, texVIndex + 0.0f);

				k += 6; // next quad

				texUIndex++;
			}
			texUIndex = 0;
			texVIndex++;
		}

		mesh->initCells(vertices, indices);

		XMFLOAT3 *positions = new XMFLOAT3[NumVertices];
		XMFLOAT3 *normals = new XMFLOAT3[NumVertices];
		XMFLOAT2 *textcoods = new XMFLOAT2[NumVertices];
		XMFLOAT3 *tangents = new XMFLOAT3[NumVertices];
		XMFLOAT3 *binormals = new XMFLOAT3[NumVertices];

		for (size_t i = 0; i < NumVertices; i++)
		{
			positions[i] = vertices[i].position;
			normals[i] = vertices[i].normal;
			textcoods[i] = vertices[i].UVs;
		}

		HRESULT res = ComputeNormals(indices.data(), NumFaces, positions, NumVertices, 0, normals);

		for (size_t i = 0; i < NumVertices; i++)
		{
			vertices[i].normal = normals[i];
		}

		size_t numfaces = vertices.size() / 3;

		res = ComputeTangentFrame(indices.data(), numfaces, positions, normals, textcoods, NumVertices, tangents, binormals);

		for (size_t i = 0; i < NumVertices; i++)
		{
			vertices[i].normal = normals[i];
			vertices[i].tangent = tangents[i];
		}

		if (normals)
		{
			delete normals;
			normals = nullptr;
		}

		if (positions)
		{
			delete positions;
			positions = nullptr;
		}

		if (textcoods)
		{
			delete textcoods;
			textcoods = nullptr;
		}

		if (tangents)
		{
			delete tangents;
			tangents = nullptr;
		}

		if (binormals)
		{
			delete binormals;
			binormals = nullptr;
		}

		if (m_heightMap)
		{
			delete[] m_heightMap;
			m_heightMap = nullptr;
		}

		StaticMeshVertexBuffer *VertexBuffer = new StaticMeshVertexBuffer();
		VertexBuffer->init(vertices);

		StaticIndexBuffer *indices_buffer = new StaticIndexBuffer();
		indices_buffer->init(indices);

		StaticMeshResources *meshResources = new StaticMeshResources(VertexBuffer, indices_buffer);

		IVertexShader *vertexShader = m_shaderManager->getVertexShader(EVertexShader::VS_TERRAIN);

		IPixelShader *pixelShader = m_shaderManager->getPixelShader(EPixelShader::PS_TERRAIN);

		mesh->getRenderData()->setResources(meshResources, vertexShader, pixelShader);

		return mesh;
	}
}
