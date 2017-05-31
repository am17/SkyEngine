#pragma once
#include "common.h"

namespace sky
{
	class Timer
	{
	public:
		Timer();
		~Timer();
		bool start();
		SKYENGINEDLL void tick();
		float getFrameTime() const;
		SKYENGINEDLL double getCurrentTime(bool queryTime = false);
		SKYENGINEDLL double getLastTime() const;
	private:
		//Частота импульсов счетчика в сенкуду
		LARGE_INTEGER m_frequency;
		float m_ticksPerMs;
		INT64 m_startTime;
		float m_frameTime;

		LARGE_INTEGER m_timerStart;
		double m_currentTime;
		double m_lastTime;
	};

	/*class Timer
	{
	public:
		Timer();
		~Timer();
		float gameTime() const;
		float deltaTime() const;
		void reset();
		bool start();
		void stop();
		void tick();
	private:
		double mSecondPerCount;
		double mDeltaTime;

		__int64 mBaseTime;
		__int64 mPausedTime;
		__int64 mStopTime;
		__int64 mPrevTime;
		__int64 mCurrTime;

		bool mStopped;
	};*/
}
