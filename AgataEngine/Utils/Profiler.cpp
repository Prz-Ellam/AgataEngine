#include "Profiler.h"
#include "Log.h"

namespace Agata {


	Profiler::Profiler(const std::string& name) : m_Name(name) {

		m_Begin = std::chrono::high_resolution_clock::now();

	}

	Profiler::~Profiler() {

		stop();
		Log::info("%s Duration: %lld nano", m_Name.c_str(), m_Diff.count());

	}

	void Profiler::stop() {

		m_End = std::chrono::high_resolution_clock::now();
		m_Diff = m_End - m_Begin;

	}

	uint64_t Profiler::getNanoSeconds() {

		return m_Diff.count();

	}

}