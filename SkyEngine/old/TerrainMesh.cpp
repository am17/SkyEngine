#include "stdafx.h"
#include "TerrainMesh.h"
#include <crtdbg.h>
#include "DirectXMesh.h"
#include "globals.h"

using namespace DirectX;

namespace sky
{
	TerrainMesh::TerrainMesh(int terrainHeight, int terrainWidth)
		: renderData(nullptr), 
		m_terrainHeight(terrainHeight),
		m_terrainWidth(terrainWidth), 
		m_terrainCells(nullptr), 
		m_cellCount(0)
	{
		renderData = new StaticMeshRenderData();
	}

	TerrainMesh::~TerrainMesh()
	{
		if (m_terrainCells)
		{
			delete[] m_terrainCells;
			m_terrainCells = nullptr;
		}

		if (renderData)
		{
			delete renderData;
			renderData = nullptr;
		}
	}

	SHeightMapPoint * TerrainMesh::loadHeightMapsFromRaw(const char* heightMapFile) const
	{
		FILE* filePtr;

		SHeightMapPoint *heightMap = new SHeightMapPoint[m_terrainWidth * m_terrainHeight];

		// Open the 16 bit raw height map file for reading in binary.
		int error = fopen_s(&filePtr, heightMapFile, "rb");
		assert(error == 0);

		// Calculate the size of the raw image data.
		unsigned long long imageSize = m_terrainHeight * m_terrainWidth;

		// Allocate memory for the raw image data.
		unsigned short* rawImage = new unsigned short[imageSize];

		// Read in the raw image data.
		unsigned long long count = fread(rawImage, sizeof(unsigned short), imageSize, filePtr);
		assert(count == imageSize);

		// Close the file.
		error = fclose(filePtr);
		assert(error == 0);

		int index = 0;
		// Copy the image data into the height map array.
		for (int j = 0; j<m_terrainHeight; j++)
		{
			for (int i = 0; i<m_terrainWidth; i++)
			{
				index = (m_terrainWidth * j) + i;

				// Store the height at this point in the height map array.
				heightMap[index].y = static_cast<float>(rawImage[index]);
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
				heightMap[index].y /= 300.0;
			}
		}

		assert(loadColorMap("Data/colormap.bmp", heightMap));

		return heightMap;
	}

	bool TerrainMesh::loadColorMap(const char* colorMapFile, SHeightMapPoint *heightMap) const
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

				heightMap[index].b = static_cast<float>(bitmapImage[k]) / 255.0f;
				heightMap[index].g = static_cast<float>(bitmapImage[k + 1]) / 255.0f;
				heightMap[index].r = static_cast<float>(bitmapImage[k + 2]) / 255.0f;

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

	TerrainCell* TerrainMesh::getCell(int index) const
	{
		return &m_terrainCells[index];
	}

	size_t TerrainMesh::getCellsCount() const
	{
		return m_cellCount;
	}

	IMaterial *TerrainMesh::getMaterial(int elementIndex) const
	{
		assert(materials.size() > elementIndex);

		return materials[elementIndex];
	}

	void TerrainMesh::setMaterial(int elementIndex, IMaterial* material)
	{
		assert(elementIndex >= 0 && material != nullptr);

		if (materials.size() <= elementIndex)
		{
			materials.push_back(material);
		}
		else
		{
			if (materials[elementIndex])
			{
				delete materials[elementIndex];
				materials[elementIndex] = nullptr;
			}

			materials[elementIndex] = material;
		}

		PSStage *psStage = renderData->getPipelineState()->getPSStageData();
		VSStage *vsStage = renderData->getPipelineState()->getVSStageData();

		MaterialObserver *observer = new MaterialObserver(*psStage, *vsStage);

		material->addObserver(observer);
	}

	StaticMeshRenderData* TerrainMesh::getRenderData() const
	{
		return renderData;
	}

	void TerrainMesh::initCells(vector<StaticMeshVertex>& verticess, vector<unsigned int>& indices)
	{
		size_t cellHeight = 33;
		size_t cellWidth = 33;

		size_t cellRowCount = (m_terrainWidth - 1) / (cellWidth - 1);

		m_cellCount = cellRowCount * cellRowCount;

		renderData->getPipelineState()->initDrawCalls(m_cellCount);

		m_terrainCells = new TerrainCell[m_cellCount];

		for (size_t j = 0; j < cellRowCount; j++)
		{
			for (size_t i = 0; i < cellRowCount; i++)
			{
				unsigned int index = (cellRowCount * j) + i;

				m_terrainCells[index].init(verticess, indices, i, j, cellHeight, cellWidth, m_terrainWidth);

				unsigned int indexCount = (cellHeight - 1) * (cellWidth - 1) * 6;
				unsigned int startIndexLocation = indexCount * index;

				renderData->getPipelineState()->getDrawCall(index)->IndexCount = indexCount;
				renderData->getPipelineState()->getDrawCall(index)->StartIndexLocation = startIndexLocation;
			}
		}
	}
}
