#pragma once

#include <type_traits>
#include <algorithm>
#include <map>

namespace Zuravvski
{
	namespace Utils
	{
		template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
		using EnumMap = std::map<T, std::string>;

		template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
		struct Enum
		{
			static constexpr T DefaultKey() noexcept
			{
				static_assert(false, "Enum<T> has not been specialized");
			}

			static constexpr std::string DefaultValue() noexcept
			{
				static_assert(false, "Enum<T> has not been specialized");
			}

			static constexpr EnumMap<T> Mapping() noexcept
			{
				static_assert(false, "Enum<T> has not been specialized");
			}
		};

		template<typename T>
		struct EnumUtils
		{
			static_assert(std::is_enum<T>::value, "T is not an enum");

			static constexpr std::string ToString(T enumeration) noexcept
			{
				auto enumMap = Enum<T>::Mapping();
				auto it = enumMap.find(enumeration);
				return it != enumMap.end() ? it->second : Enum<T>::DefaultValue();
			}

			static constexpr T FromString(const std::string& value) noexcept
			{
				auto enumMap = Enum<T>::Mapping();
				auto foundIt = std::find_if(enumMap.begin(), enumMap.end(), [&value](const std::pair<T, std::string>& kv)
				{
					return value == kv.second;
				});
				return foundIt != enumMap.end() ? foundIt->first : Enum<T>::DefaultKey();
			}
		};

		template<typename T>
		constexpr std::string EnumToString(T enumeration) noexcept
		{
			return EnumUtils<T>::ToString(enumeration);
		}

		template<typename T>
		constexpr T StringToEnum(const std::string& enumerationName) noexcept
		{
			return EnumUtils<T>::FromString(enumerationName);
		}
	}
}