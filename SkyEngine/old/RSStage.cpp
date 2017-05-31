#include "stdafx.h"
#include "RSStage.h"

namespace sky
{
	RSStage::RSStage()
		:m_cullBackface(true)
	{
	}


	RSStage::~RSStage()
	{
	}

	void RSStage::setBackfaceCulling(bool cullBackface)
	{
		m_cullBackface = cullBackface;
	}

	bool RSStage::getBackfaceCulling() const
	{
		return m_cullBackface;
	}
}
