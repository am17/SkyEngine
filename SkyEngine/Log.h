#pragma once
#include <iostream>

using namespace std;

namespace sky
{
	class Log
	{
	public:
		static void write(const char* message);
		static void writeWithTime(const char* message, double time);
	};

	inline void Log::write(const char* message)
	{
		std::cout << message << endl;
	}

	inline void Log::writeWithTime(const char* message, double time)
	{
		std::cout << message << ": " << time << endl;
	}
}
