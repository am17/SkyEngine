#pragma once

namespace sky
{
	class RSStage
	{
	public:
		RSStage();
		~RSStage();
		void setBackfaceCulling(bool cullBackface = true);
		bool getBackfaceCulling() const;
	private:
		bool m_cullBackface;
		void *viewPorts;
		size_t viewPortCount;
		void *scissorRects;
		size_t scissorRectsCount;
	};
}

