#pragma once
#include <DirectXMath.h>
#include "common.h"

using namespace DirectX;

namespace sky
{
	class Behavior
	{
	public:
		SKYENGINEDLL Behavior();
		SKYENGINEDLL ~Behavior();
		void update(XMFLOAT4X4 &matrix);
	private:
		float m_rotation;
	};
}

