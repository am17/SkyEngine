#pragma once
#include "IAStage.h"
#include "VSStage.h"
#include "PSStage.h"
#include "RSStage.h"
#include "OMStage.h"

namespace sky
{
	struct SDrawCall;

	class D3D11PipelineState
	{
	public:
		D3D11PipelineState();
		~D3D11PipelineState();
		IAStage *getIAStageData() const;
		VSStage *getVSStageData() const;
		PSStage *getPSStageData() const;
		RSStage *getRSStageData() const;
		OMStage *getOMStageData() const;
		bool isReady() const;
		void setReady(bool isReady);
		bool initIAStage();
		bool initVSStage();
		bool initPSStage();
		bool initRSStage();
		bool initOMStage();
		size_t getDrawCallsCount() const;
		void initDrawCalls(size_t drawCommandCount);
		SDrawCall *getDrawCalls() const;
		SDrawCall *getDrawCall(unsigned int callIndex) const;
	private:
		bool m_readyForRender;
		IAStage *m_IAStageData;
		VSStage *m_VSStageData;
		PSStage *m_PSStageData;
		RSStage *m_RSStageData;
		OMStage *m_OMStageData;
		size_t m_drawCommandCount;
		SDrawCall *m_drawCommands;
	};

	struct SDrawCall
	{
		SDrawCall() 
			: IndexCount(0),
			StartIndexLocation(0),
			BeCalled(false),
			InstanceCount(1)
		{}
		SDrawCall(unsigned int indexCount, int startIndexLocation, bool beCalled, unsigned int instanceCount = 1)
			: IndexCount(indexCount),
			StartIndexLocation(startIndexLocation),
			BeCalled(beCalled),
			InstanceCount(instanceCount)
		{}
		unsigned int IndexCount;
		unsigned int StartIndexLocation;
		bool BeCalled;
		unsigned int InstanceCount;
	};
}

