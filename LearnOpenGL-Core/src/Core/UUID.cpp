#include "Core/UUID.h"

#include <random>

static std::random_device s_seeder;
static auto seed = s_seeder.entropy() ? s_seeder() : time(nullptr);
static std::mt19937 s_engine(seed);
static std::uniform_int_distribution<long> s_distribution;

UUID::UUID()
	: m_UUID(s_distribution(s_engine))
{
}
