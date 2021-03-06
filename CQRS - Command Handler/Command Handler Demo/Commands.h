#pragma once

#include <string>

namespace Zuravvski
{
	namespace Demo
	{
		struct ICommand
		{
			// Marker interface
			virtual ~ICommand() noexcept = default;
		};

		struct Register : ICommand
		{
			std::string login;
			std::string password;
			std::string salt;
		};

		struct BrowseUsers : ICommand
		{
			// Parameterless command
		};
	}
}