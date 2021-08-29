#ifndef AGATA_PROFILER_H
#define AGATA_PROFILER_H

#include <string>
#include <chrono>

/*
	Trabajando en esto, se espera ser el reemplazo de Timer
*/
namespace Agata {

	class Profiler {
	public:
		Profiler(const std::string& name);
		~Profiler();
		void stop();
		uint64_t getNanoSeconds();
	private:
		std::string m_Name;
		std::chrono::steady_clock::time_point m_Begin, m_End;
		std::chrono::nanoseconds m_Diff;
	};

}

#endif