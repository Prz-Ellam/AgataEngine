#ifndef AGATA_TIMER_H
#define AGATA_TIMER_H

#include <Windows.h>
#include <string>

namespace Agata {

	class Timer {
	public:
		Timer(const std::string& name);
		virtual ~Timer();
		void begin();
		void end();
	private:
		double getPerformanceFrequency();
		std::string m_Name;
		double m_PerformanceCounter;
		LARGE_INTEGER m_BeginTime, m_EndTime;
	};

}

#define AGATA_PROFILE() Timer time(__FUNCTION__)

#endif