#pragma once

#include "EnumUtils.h"

enum class SampleEnum
{
	Tic,
	Tac,
	Toe
};

template<>
struct Zuravvski::Utils::Enum<SampleEnum>
{
	static constexpr SampleEnum DefaultKey() noexcept
	{
		return SampleEnum::Tic;
	}

	static std::string DefaultValue() noexcept
	{
		return "Tic";
	}

	static EnumMap<SampleEnum> Mapping() noexcept
	{
		static EnumMap<SampleEnum> enumMap;
		static bool initialized = false;

		if (!initialized)
		{
			enumMap.insert({ SampleEnum::Tic, "Tic" });
			enumMap.insert({ SampleEnum::Tac, "Tac" });
			enumMap.insert({ SampleEnum::Toe, "Toe" });
			initialized = true;
		}

		return enumMap;
	}
};