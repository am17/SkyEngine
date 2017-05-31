#pragma once

namespace sky
{
	class OMStage
	{
	public:
		OMStage();
		~OMStage();
		void setZBufferEnabled(bool Enabled = true);
		bool getZbufferEnabled() const;
	private:
		bool m_ZBufferEnabled;
		void *blendState;
		void *renderTarget;
	};
}

