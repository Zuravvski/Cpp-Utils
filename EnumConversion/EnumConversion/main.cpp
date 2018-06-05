#include <iostream>
#include <string>
#include "SampleEnum.h"

int main()
{
	using namespace Zuravvski::Utils;

	/*
	 * Enum -> String 
	 */

	// Explicit invokation
	std::cout << "1. Sample enum value to string: " << EnumUtils<SampleEnum>::ToString(SampleEnum::Tac) << std::endl;

	// Implicit invokation (shorthand for the above)
	std::cout << "2. Sample enum value to string: " << EnumToString(SampleEnum::Tic) << std::endl;

	/*
	 * String -> Enum
	 */

	// Explicit invokation
	const auto tac = EnumUtils<SampleEnum>::FromString("Tac");
	_ASSERT(tac == SampleEnum::Tac);

	// Implicit invokation (shorthand for the above)
	const auto tacShort = StringToEnum<SampleEnum>("Tac");
	_ASSERT(tacShort == SampleEnum::Tac);

	// If the value is not recognized it would be parsed to a default value
	const auto incorrect = StringToEnum<SampleEnum>("Some random stuff"); // should return Tic, which was specified as a default
	_ASSERT(incorrect == SampleEnum::Tic);

	std::cin.get();
}
