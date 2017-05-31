#include "stdafx.h"
#include "D3D11PipelineState.h"
//#include <algorithm>
//#include <memory>

namespace sky
{
	D3D11PipelineState::D3D11PipelineState()
		:m_readyForRender(false),
		m_IAStageData(nullptr),
		m_VSStageData(nullptr),
		m_PSStageData(nullptr),
		m_RSStageData(nullptr),
		m_OMStageData(nullptr),
		m_drawCommands(nullptr),
		m_drawCommandCount(1)
	{
		D3D11PipelineState::initIAStage();
		D3D11PipelineState::initVSStage();
		D3D11PipelineState::initPSStage();
		D3D11PipelineState::initRSStage();
		D3D11PipelineState::initOMStage();
	}

	D3D11PipelineState::~D3D11PipelineState()
	{
		if (m_drawCommands)
		{
			delete[] m_drawCommands;
			m_drawCommands = nullptr;
		}

		if (m_IAStageData)
		{
			delete m_IAStageData;
			m_IAStageData = nullptr;
		}

		if (m_VSStageData)
		{
			delete m_VSStageData;
			m_VSStageData = nullptr;
		}

		if (m_PSStageData)
		{
			delete m_PSStageData;
			m_PSStageData = nullptr;
		}

		if (m_RSStageData)
		{
			delete m_RSStageData;
			m_RSStageData = nullptr;
		}
	}
	IAStage *D3D11PipelineState::getIAStageData() const
	{
		return m_IAStageData;
	}
	VSStage *D3D11PipelineState::getVSStageData() const
	{
		return m_VSStageData;
	}
	PSStage *D3D11PipelineState::getPSStageData() const
	{
		return m_PSStageData;
	}
	RSStage *D3D11PipelineState::getRSStageData() const
	{
		return m_RSStageData;
	}
	OMStage *D3D11PipelineState::getOMStageData() const
	{
		return m_OMStageData;
	}
	bool D3D11PipelineState::isReady() const
	{
		return m_readyForRender;
	}
	void D3D11PipelineState::setReady(bool isReady)
	{
		m_readyForRender = isReady;
	}

	bool D3D11PipelineState::initIAStage()
	{
		if (m_IAStageData == nullptr)
		{
			m_IAStageData = new IAStage();
		}

		return m_IAStageData != nullptr;
	}
	bool D3D11PipelineState::initVSStage()
	{
		if (m_VSStageData == nullptr)
		{
			m_VSStageData = new VSStage();
		}

		return m_VSStageData != nullptr;
	}
	bool D3D11PipelineState::initPSStage()
	{
		if (m_PSStageData == nullptr)
		{
			m_PSStageData = new PSStage();
		}

		return m_PSStageData != nullptr;
	}
	bool D3D11PipelineState::initRSStage()
	{
		if (m_RSStageData == nullptr)
		{
			m_RSStageData = new RSStage();
		}

		return m_RSStageData != nullptr;
	}

	bool D3D11PipelineState::initOMStage()
	{
		if (m_OMStageData == nullptr)
		{
			m_OMStageData = new OMStage();
		}

		return m_OMStageData != nullptr;
	}

	size_t D3D11PipelineState::getDrawCallsCount() const
	{
		return m_drawCommandCount;
	}

	void D3D11PipelineState::initDrawCalls(size_t drawCommandCount)
	{
		m_drawCommandCount = drawCommandCount;

		m_drawCommands = new SDrawCall[drawCommandCount];
	}

	SDrawCall* D3D11PipelineState::getDrawCalls() const
	{
		assert(m_drawCommands != nullptr);

		return m_drawCommands;
	}

	SDrawCall* D3D11PipelineState::getDrawCall(unsigned int callIndex) const
	{
		assert(m_drawCommands != nullptr && callIndex < m_drawCommandCount);

		return &m_drawCommands[callIndex];
	}
}
