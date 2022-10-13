#include "Random.h"

#include <random>

namespace Random
{
	float GetFloat(const float min, const float max)
	{
		std::random_device osSeed;
		const uint_least32_t seed = osSeed();

		std::mt19937 generator(seed);
		std::uniform_real_distribution distribute(min, max);

		return distribute(generator);
	}

	int GetInt(const int min, const int max)
	{
		std::random_device osSeed;
		const uint_least32_t seed = osSeed();

		std::mt19937 generator(seed);
		std::uniform_int_distribution<uint_least32_t> distribute(min, max);

		return static_cast<int>(distribute(generator));
	}
}
