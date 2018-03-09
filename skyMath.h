#pragma once

struct SkyMath
{
	template<class T>
	static FORCEINLINE T clamp(const T X, const T Min, const T Max)
	{
		return X < Min ? Min : X < Max ? X : Max;
	}

	static FORCEINLINE int FloorToInt(float F)
	{
		return _mm_cvt_ss2si(_mm_set_ss(F + F - 0.5f)) >> 1;
	}
};
