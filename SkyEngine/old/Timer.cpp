#include "stdafx.h"
#include "Timer.h"

namespace sky
{
	Timer::Timer():  
		m_ticksPerMs(0), 
		m_startTime(0), 
		m_frameTime(0), 
		m_currentTime(0), 
		m_lastTime(0)
	{
	}


	Timer::~Timer()
	{
	}

	bool Timer::start()
	{
		//Получаем частоту импульсов счетчика в секундах
		QueryPerformanceFrequency(&m_frequency);

		INT64 frequency = m_frequency.QuadPart;

		if (frequency == 0)
		{
			return false;
		}

		m_ticksPerMs = static_cast<float>(frequency / 1000);

		//Получаем тукущее значение счетчика
		QueryPerformanceCounter(&m_timerStart);

		INT64 currentTime = m_timerStart.QuadPart;

		m_startTime = currentTime;

		return true;
	}

	void Timer::tick()
	{
		LARGE_INTEGER timerStop;

		QueryPerformanceCounter(&timerStop);

		//INT64 currentTime = timerStop.QuadPart;

		//float timeDifference = static_cast<float>(currentTime - m_startTime);

		//m_frameTime = timeDifference / m_ticksPerMs;

		//m_startTime = currentTime;

		m_lastTime = m_currentTime;

		m_currentTime = (static_cast<double>(timerStop.QuadPart - m_timerStart.QuadPart) / m_frequency.QuadPart);
	}

	float Timer::getFrameTime() const
	{
		return m_frameTime;
	}

	double Timer::getCurrentTime(bool queryTime)
	{
		if (queryTime)
		{
			LARGE_INTEGER timerStop;

			QueryPerformanceCounter(&timerStop);

			m_currentTime = (static_cast<double>(timerStop.QuadPart - m_timerStart.QuadPart) / m_frequency.QuadPart);
		}

		return m_currentTime;
	}

	double Timer::getLastTime() const
	{
		return m_lastTime;
	}

	/*Timer::Timer()
		: mSecondPerCount(0), 
		mDeltaTime(0), 
		mBaseTime(0), 
		mPausedTime(0), 
		mStopTime(0), 
		mPrevTime(0), 
		mCurrTime(0), 
		mStopped(false)
	{
		__int64 countsPerSec;

		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
		mSecondPerCount = 1.0f / (float)countsPerSec;
	}

	Timer::~Timer()
	{
	}

	float Timer::gameTime() const
	{
		if (mStopped)
		{
			return (float)(((mStopTime - mPausedTime) - mBaseTime) - mSecondPerCount);
		}
		else
		{
			return (float)(((mCurrTime - mPausedTime) - mBaseTime) - mSecondPerCount);
		}
	}

	float Timer::deltaTime() const
	{
		return (float)mDeltaTime;
	}

	void Timer::reset()
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mBaseTime = currTime;
		mBaseTime = currTime;

		mStopTime = 0;
		mStopped = false;
	}

	bool Timer::start()
	{
		__int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

		if (mStopped)
		{
			mPausedTime += (startTime - mStopTime);
			mPrevTime = startTime;
			mStopTime = 0;
			mStopped = false;

			return false;
		}

		return true;
	}

	void Timer::stop()
	{
		if (!mStopped)
		{
			__int64 currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

			mStopTime = currTime;
			mStopped = true;
		}
	}

	void Timer::tick()
	{
		if (mStopped)
		{
			mDeltaTime = 0.0f;
			return;
		}

		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mCurrTime = currTime;

		mDeltaTime = (mCurrTime - mPrevTime) * mSecondPerCount;

		mPrevTime = mCurrTime;

		if (mDeltaTime < 0.0f)
		{
			mDeltaTime = 0.0f;
		}
	}*/
}
