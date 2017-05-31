#include "stdafx.h"
#include "TerrainGeometry.h"
#include <crtdbg.h>
#include "DirectXMesh.h"
#include "globals.h"

using namespace DirectX;

namespace sky
{
	TerrainGeometry::TerrainGeometry(int terrainHeight, int terrainWidth)
		: m_heightMap(nullptr), 
		m_terrainHeight(terrainHeight),
		m_terrainWidth(terrainWidth), 
		m_terrainCells(nullptr), 
		m_cellCount(0)
	{
	}


	TerrainGeometry::~TerrainGeometry()
	{
		if (m_terrainCells)
		{
			delete[] m_terrainCells;
			m_terrainCells = nullptr;
		}

		if (m_heightMap)
		{
			delete[] m_heightMap;
			m_heightMap = nullptr;
		}
	}

	bool TerrainGeometry::loadHeightMapsFromBitmap(const char* heightMapFile)
	{
		m_heightMap = new SHeightMapPoint[m_terrainWidth * m_terrainHeight];

		FILE* filePtr;
		BITMAPFILEHEADER bitmapFileHeader;
		BITMAPINFOHEADER bitmapInfoHeader;

		int error = fopen_s(&filePtr, heightMapFile, "rb");
		if (error != 0)
		{
			return false;
		}

		unsigned long long count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

		if (count != 1)
		{
			return false;
		}

		count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
		if (count != 1)
		{
			return false;
		}

		// Make sure the height map dimensions are the same as the terrain dimensions for easy 1 to 1 mapping.
		if ((bitmapInfoHeader.biHeight != m_terrainHeight) || (bitmapInfoHeader.biWidth != m_terrainWidth))
		{
			return false;
		}

		int imageSize = m_terrainHeight * ((m_terrainWidth * 3) + 1);

		unsigned char* bitmapImage = new unsigned char[imageSize];

		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

		count = fread(bitmapImage, 1, imageSize, filePtr);
		if (count != imageSize)
		{
			return false;
		}

		error = fclose(filePtr);
		if (error != 0)
		{
			return false;
		}

		int k = 0;
		int index = 0;
		unsigned char height;
		for (int j = 0; j<m_terrainHeight; j++)
		{
			for (int i = 0; i<m_terrainWidth; i++)
			{
				// Bitmaps are upside down so load bottom to top into the height map array.
				index = (m_terrainWidth * (m_terrainHeight - 1 - j)) + i;

				// Get the grey scale pixel value from the bitmap image data at this location.
				height = bitmapImage[k];

				// Store the pixel value as the height at this point in the height map array.
				m_heightMap[index].y = static_cast<float>(height);

				// Increment the bitmap image data index.
				k += 3;
			}

			// Compensate for the extra byte at end of each line in non-divide by 2 bitmaps (eg. 513x513).
			k++;
		}

		for (int j = 0; j<m_terrainHeight; j++)
		{
			for (int i = 0; i<m_terrainWidth; i++)
			{
				index = (m_terrainWidth * j) + i;

				// Scale the height.
				m_heightMap[index].y /= 12.0;
			}
		}

		if (bitmapImage)
		{
			delete[] bitmapImage;
			bitmapImage = nullptr;
		}

		if (!loadColorMap("Textures/colormap.bmp"))
		{
			return false;
		} 

		return true;
	}

	bool TerrainGeometry::loadHeightMapsFromRaw(const char* heightMapFile)
	{
		FILE* filePtr;

		// Create the float array to hold the height map data.
		m_heightMap = new SHeightMapPoint[m_terrainWidth * m_terrainHeight];
		if (!m_heightMap)
		{
			return false;
		}

		// Open the 16 bit raw height map file for reading in binary.
		int error = fopen_s(&filePtr, heightMapFile, "rb");
		if (error != 0)
		{
			return false;
		}

		// Calculate the size of the raw image data.
		unsigned long long imageSize = m_terrainHeight * m_terrainWidth;

		// Allocate memory for the raw image data.
		unsigned short* rawImage = new unsigned short[imageSize];
		if (!rawImage)
		{
			return false;
		}

		// Read in the raw image data.
		unsigned long long count = fread(rawImage, sizeof(unsigned short), imageSize, filePtr);
		if (count != imageSize)
		{
			return false;
		}

		// Close the file.
		error = fclose(filePtr);
		if (error != 0)
		{
			return false;
		}

		int index = 0;
		// Copy the image data into the height map array.
		for (int j = 0; j<m_terrainHeight; j++)
		{
			for (int i = 0; i<m_terrainWidth; i++)
			{
				index = (m_terrainWidth * j) + i;

				// Store the height at this point in the height map array.
				m_heightMap[index].y = static_cast<float>(rawImage[index]);
			}
		}

		if (rawImage)
		{
			delete[] rawImage;
			rawImage = nullptr;
		}
		
		for (int j = 0; j< m_terrainHeight; j++)
		{
			for (int i = 0; i< m_terrainWidth; i++)
			{
				index = (m_terrainWidth * j) + i;

				// Scale the height.
				m_heightMap[index].y /= 300.0;
			}
		}

		if (!loadColorMap("Data/colormap.bmp"))
		{
			return false;
		}

		return true;
	}

	void TerrainGeometry::createMesh(vector<StaticMeshVertex>& vertices, vector<unsigned int>& indices)
	{
		int cols = m_terrainHeight;
		int rows = m_terrainWidth;

		size_t NumVertices = rows * cols;
		size_t NumFaces = (rows - 1)*(cols - 1) * 2;

		vertices.reserve(NumVertices);
		vertices.resize(NumVertices);

		indices.reserve(NumFaces * 3);
		indices.resize(NumFaces * 3);

		for (DWORD i = 0; i < rows; ++i)
		{
			for (DWORD j = 0; j < cols; ++j)
			{
				int index = (m_terrainWidth * j) + i;

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
				indices[k] = i*cols + j;  
				vertices[i*cols + j].UVs = XMFLOAT2(texUIndex + 0.0f, texVIndex + 1.0f);

				indices[k + 1] = i*cols + j + 1;  
				vertices[i*cols + j + 1].UVs = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

				indices[k + 2] = (i + 1)*cols + j;  
				vertices[(i + 1)*cols + j].UVs = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);


				indices[k + 3] = (i + 1)*cols + j; 
				vertices[(i + 1)*cols + j].UVs = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);

				indices[k + 4] = i*cols + j + 1; 
				vertices[i*cols + j + 1].UVs = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

				indices[k + 5] = (i + 1)*cols + j + 1;  
				vertices[(i + 1)*cols + j + 1].UVs = XMFLOAT2(texUIndex + 1.0f, texVIndex + 0.0f);

				k += 6; // next quad

				texUIndex++;
			}
			texUIndex = 0;
			texVIndex++;
		}

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
			vertices[i].binormal = binormals[i];
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

		initCells(vertices, indices);
	}

	bool TerrainGeometry::loadColorMap(const char* colorMapFile) const
	{
		int error, imageSize, i, j, k, index;
		FILE* filePtr;
		unsigned long long count;
		BITMAPFILEHEADER bitmapFileHeader;
		BITMAPINFOHEADER bitmapInfoHeader;
		unsigned char* bitmapImage;


		// Open the color map file in binary.
		error = fopen_s(&filePtr, colorMapFile, "rb");
		if (error != 0)
		{
			return false;
		}

		// Read in the file header.
		count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
		if (count != 1)
		{
			return false;
		}

		// Read in the bitmap info header.
		count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
		if (count != 1)
		{
			return false;
		}

		// Make sure the color map dimensions are the same as the terrain dimensions for easy 1 to 1 mapping.
		if ((bitmapInfoHeader.biWidth != m_terrainWidth) || (bitmapInfoHeader.biHeight != m_terrainHeight))
		{
			return false;
		}

		// Calculate the size of the bitmap image data.  Since this is non-divide by 2 dimensions (eg. 257x257) need to add extra byte to each line.
		imageSize = m_terrainHeight * ((m_terrainWidth * 3) + 1);

		// Allocate memory for the bitmap image data.
		bitmapImage = new unsigned char[imageSize];
		if (!bitmapImage)
		{
			return false;
		}

		// Move to the beginning of the bitmap data.
		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

		// Read in the bitmap image data.
		count = fread(bitmapImage, 1, imageSize, filePtr);
		if (count != imageSize)
		{
			return false;
		}

		// Close the file.
		error = fclose(filePtr);
		if (error != 0)
		{
			return false;
		}

		// Initialize the position in the image data buffer.
		k = 0;

		// Read the image data into the color map portion of the height map structure.
		for (j = 0; j<m_terrainHeight; j++)
		{
			for (i = 0; i<m_terrainWidth; i++)
			{
				// Bitmaps are upside down so load bottom to top into the array.
				index = (m_terrainWidth * (m_terrainHeight - 1 - j)) + i;

				m_heightMap[index].b = static_cast<float>(bitmapImage[k]) / 255.0f;
				m_heightMap[index].g = static_cast<float>(bitmapImage[k + 1]) / 255.0f;
				m_heightMap[index].r = static_cast<float>(bitmapImage[k + 2]) / 255.0f;

				k += 3;
			}

			// Compensate for extra byte at end of each line in non-divide by 2 bitmaps (eg. 257x257).
			k++;
		}

		if (bitmapImage)
		{
			delete[] bitmapImage;
			bitmapImage = nullptr;
		}

		return true;
	}

	TerrainCell* TerrainGeometry::getCell(int index) const
	{
		return &m_terrainCells[index];
	}

	void TerrainGeometry::initCells(vector<StaticMeshVertex>& verticess, vector<unsigned int>& indicess)
	{
		size_t cellHeight = 33;
		size_t cellWidth = 33;

		size_t cellRowCount = (m_terrainWidth - 1) / (cellWidth - 1);

		m_cellCount = cellRowCount * cellRowCount;

		m_terrainCells = new TerrainCell[m_cellCount];

		for (size_t j = 0; j < cellRowCount; j++)
		{
			for (size_t i = 0; i < cellRowCount; i++)
			{
				unsigned int index = (cellRowCount * j) + i;

				m_terrainCells[index].init(verticess, i, j, cellHeight, cellWidth, m_terrainWidth);
			}
		}
	}

	TerrainCell::TerrainCell(): 
		m_renderData(nullptr), 
		m_vertexCount(0), 
		m_indexCount(0)
	{
	}

	TerrainCell::~TerrainCell()
	{
		if (m_renderData)
		{
			delete m_renderData;
			m_renderData = nullptr;
		}
	}

	bool TerrainCell::init(vector<StaticMeshVertex>& verticess, size_t nodeIndexX, size_t nodeIndexY, size_t cellHeight, size_t cellWidth, int terrainWidth)
	{
		m_vertexCount = cellHeight * cellWidth;
		m_indexCount = (cellHeight - 1) * (cellWidth - 1) * 6;

		vector<StaticMeshVertex> vertices;
		vector<unsigned int> indices(m_indexCount);

		int cellVertexIndex = (nodeIndexX * (cellWidth - 1)) + (nodeIndexY * (cellHeight - 1) * (terrainWidth - 1));

		int index = 0;

		float maxX = FLT_MIN;
		float minX = FLT_MAX;
		float maxY = FLT_MIN;
		float minY = FLT_MAX;
		float maxZ = FLT_MIN;
		float minZ = FLT_MAX;
		for (size_t j = 0; j< cellHeight; j++)
		{
			for (size_t i = 0; i< cellWidth; i++)
			{
				StaticMeshVertex vertex = verticess[cellVertexIndex];

				float x = vertex.position.x;
				float y = vertex.position.y;
				float z = vertex.position.z;

				if (x > maxX)  maxX = x;
				if (x < minX)  minX = x;
				if (y > maxY)  maxY = y;
				if (y < minY)  minY = y;
				if (z > maxZ)  maxZ = z;
				if (z < minZ)  minZ = z;

				vertices.push_back(vertex);
				cellVertexIndex++;
				index++;
			}
			cellVertexIndex += terrainWidth - cellWidth;
		}

		int k = 0;
		for (DWORD i = 0; i < cellHeight - 1; i++)
		{
			for (DWORD j = 0; j < cellWidth - 1; j++)
			{
				indices[k] = i*cellWidth + j;

				indices[k + 1] = i*cellWidth + j + 1;

				indices[k + 2] = (i + 1)*cellWidth + j;


				indices[k + 3] = (i + 1)*cellWidth + j;

				indices[k + 4] = i*cellWidth + j + 1;

				indices[k + 5] = (i + 1)*cellWidth + j + 1;

				k += 6; // next quad
			}
		}

		XMFLOAT3 bbCenter = XMFLOAT3((maxX - minX) / 2, (maxY - minY) / 2, (maxZ - minZ) / 2);
		XMFLOAT3 extents = XMFLOAT3(cellWidth - 1, maxY - minY, cellHeight - 1);

		m_AABB = BoundingBox(bbCenter, extents);

		StaticMeshVertexBuffer *VertexBuffer = new StaticMeshVertexBuffer();
		VertexBuffer->init(vertices);

		StaticIndexBuffer *indices_buffer = new StaticIndexBuffer();
		indices_buffer->init(indices);

		StaticMeshResources *meshResources = new StaticMeshResources(VertexBuffer, indices_buffer);

		m_renderData = new StaticMeshRenderData();

		ShaderManager *shaderManager = global::getShaderManager();

		assert(shaderManager != nullptr);

		IVertexShader *vertexShader = shaderManager->getVertexShader(EVertexShader::VS_TERRAIN);

		IPixelShader *pixelShader = shaderManager->getPixelShader(EPixelShader::PS_TERRAIN);

		m_renderData->setResources(meshResources, vertexShader, pixelShader);

		return true;
	}

	StaticMeshRenderData* TerrainCell::getRenderData() const
	{
		return m_renderData;
	}

	const BoundingBox* TerrainCell::getBoundingBox() const
	{
		return &m_AABB;
	}
}
