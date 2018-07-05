#include <iostream>
#include "CommandBus.h"
#include <Hypodermic/ContainerBuilder.h>

std::shared_ptr<Hypodermic::Container> CreateContainer()
{
	Hypodermic::ContainerBuilder containerBuilder;
	containerBuilder.registerType<InMemoryUserRepository>().as<IUserRepository>().singleInstance();
	containerBuilder.registerType<RegisterHandler>().as<ICommandHandler<Register>>();
	containerBuilder.registerType<BrowseUsersHandler>().as<ICommandHandler<BrowseUsers>>();
	return containerBuilder.build();
}

auto main(int argc, char** argv) -> int
{
	const auto container = CreateContainer();

	// Create the bus
	const auto commandBus = std::make_unique<CommandBus>(container);

	try
	{
		// Create the sample command
		auto registerCommand = std::make_unique<Register>();
		registerCommand->login = "John";
		registerCommand->password = "Secret123$";

		// Put the command on the bus
		commandBus->Dispatch(std::move(registerCommand)).get();

		// Verify
		std::cout << "\nVerification: " << std::endl;
		commandBus->Dispatch(std::make_unique<BrowseUsers>()).get();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	std::cin.get();
}