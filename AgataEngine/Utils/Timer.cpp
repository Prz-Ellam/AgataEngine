#include "Timer.h"
#include "Log.h"
#include "pch.h"

namespace Agata {

	Timer::Timer(const std::string& name) {

		m_Name = name;
		begin();

	}

	Timer::~Timer() {

		end();

	}

	void Timer::begin() {

		QueryPerformanceCounter(&m_BeginTime);
		m_PerformanceCounter = getPerformanceFrequency();

	}

	void Timer::end() {

		QueryPerformanceCounter(&m_EndTime);
		double duration = (double)((m_EndTime.QuadPart - m_BeginTime.QuadPart) * 1000.0) / m_PerformanceCounter;
		Log::info("%s Duration: %.5lf ms", m_Name.c_str(), duration);

	}

	double Timer::getPerformanceFrequency() {

		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		return (double)frequency.QuadPart;
	
	}

}