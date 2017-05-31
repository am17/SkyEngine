#include "stdafx.h"
#include "Behavior.h"

namespace sky
{
	Behavior::Behavior()
		:m_rotation(0)
	{
	}


	Behavior::~Behavior()
	{
	}

	void Behavior::update(XMFLOAT4X4 &matrix)
	{
		m_rotation += XM_PI * 0.0005f;
		if (m_rotation > 360.0f)
		{
			m_rotation -= 360.0f;
		}

		XMMATRIX rotMat = XMMatrixRotationX(0) * XMMatrixRotationY(m_rotation) * XMMatrixRotationZ(0);

		XMStoreFloat4x4(&matrix, rotMat);
	}
}
