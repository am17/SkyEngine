#include "stdafx.h"
#include "VertexInputLayout.h"

namespace sky
{
	VertexInputLayout::VertexInputLayout()
		:m_layout(nullptr)
	{
	}

	VertexInputLayout::~VertexInputLayout()
	{
		if (m_layout)
		{
			m_layout->Release();
			m_layout = nullptr;
		}
	}

	void VertexInputLayout::setData(ID3D11InputLayout *data)
	{
		m_layout = data;
	}

	ID3D11InputLayout* VertexInputLayout::getData() const
	{
		return m_layout;
	}
}
