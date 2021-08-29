#ifndef AGATA_RANDOM_H
#define AGATA_RANDOM_H

#include <random>

namespace Agata {

	class Random {
	public:
		static void init();
		static float Float();
		static int Int();
		static int Int(int minRange, int maxRange);
	private:
		static std::mt19937 s_Randomizer;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

}

#endif