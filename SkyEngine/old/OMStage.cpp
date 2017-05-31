#include "stdafx.h"
#include "OMStage.h"

namespace sky
{
	OMStage::OMStage()
		:m_ZBufferEnabled(true),
		blendState(nullptr),
		renderTarget(nullptr)
	{
	}

	OMStage::~OMStage()
	{
	}

	void OMStage::setZBufferEnabled(bool Enabled)
	{
		m_ZBufferEnabled = Enabled;
	}

	bool OMStage::getZbufferEnabled() const
	{
		return m_ZBufferEnabled;
	}
}
