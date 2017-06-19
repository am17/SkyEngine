#pragma once

namespace sky
{
	enum class EDepthState
	{
		DS_DEFAULT = 0,
		DS_NONE = 1,
		DS_LESS_EQUAL = 2
	};

	enum class EBlendState
	{
		BS_OPAQUE = 0,
		BS_ALPHA_BLENDING = 1,
		BS_NO_BLENDING = 2,
		BS_BLENDING_ADD = 3
	};
}