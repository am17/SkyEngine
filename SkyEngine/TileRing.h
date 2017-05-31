#pragma once
#include "Device.h"
#include <DirectXMath.h>

using namespace DirectX;

struct InstanceData
{
	InstanceData(){}
	InstanceData(float x, float y)
		:position(x, y) {}
	XMFLOAT2 position;
};

class TileRing
{
public:
	TileRing(int holeWidth, int outerWidth, float tileSize, Device *aDevice)
		:_pDevice(aDevice),
		m_holeWidth(holeWidth),
		m_outerWidth(outerWidth),
		m_ringWidth((outerWidth - holeWidth) / 2),
		m_nTiles(outerWidth*outerWidth - holeWidth*holeWidth),
		m_tileSize(tileSize)
	{
		assert((outerWidth - holeWidth) % 2 == 0);

		createInstanceDataVB();
	}

	int   outerWidth() const { return m_outerWidth; }
	int   nTiles()     const { return m_nTiles; }
	float tileSize()   const { return m_tileSize; }
	void setRenderingState()  const
	{
		_pVertexBuffer->bind(0);
	}
private:
	TileRing(const TileRing&);
	TileRing& operator=(const TileRing&);
	void createInstanceDataVB()
	{
		int index = 0;
		m_pVBData = new InstanceData[m_nTiles];

		const float halfWidth = 0.5f * (float)m_outerWidth;
		for (int y = 0; y < m_outerWidth; ++y)
		{
			for (int x = 0; x < m_outerWidth; ++x)
			{
				if (InRing(x, y))
				{
					m_pVBData[index].position.x = m_tileSize * ((float)x - halfWidth);
					m_pVBData[index].position.y = m_tileSize * ((float)y - halfWidth);
					index++;
				}
			}
		}
		assert(index == m_nTiles);

		_pVertexBuffer = _pDevice->createVertexBuffer(m_pVBData, m_nTiles, sizeof(InstanceData));

		if (m_pVBData)
		{
			delete[] m_pVBData;
			m_pVBData = nullptr;
		}
	}
	bool InRing(int x, int y) const
	{
		assert(x >= 0 && x < m_outerWidth);
		assert(y >= 0 && y < m_outerWidth);
		return (x < m_ringWidth || y < m_ringWidth || x >= m_outerWidth - m_ringWidth || y >= m_outerWidth - m_ringWidth);
	}
	Device *_pDevice;

	const int m_holeWidth, m_outerWidth, m_ringWidth;
	const int m_nTiles;
	const float m_tileSize;
	InstanceData* m_pVBData;
	VertexBuffer *_pVertexBuffer;
};