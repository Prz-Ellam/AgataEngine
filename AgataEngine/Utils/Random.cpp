#include "Random.h"

namespace Agata {

	std::mt19937 Random::s_Randomizer;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

	void Random::init() {

		s_Randomizer.seed(std::random_device()());

	}

	float Random::Float() {

		return s_Distribution(s_Randomizer) / (float)std::numeric_limits<uint32_t>::max();

	}

	int Random::Int() {

		return s_Distribution(s_Randomizer);

	}

	int Random::Int(int minRange, int maxRange) {

		return s_Distribution(s_Randomizer) % (maxRange - minRange + 1) + minRange;

	}

}