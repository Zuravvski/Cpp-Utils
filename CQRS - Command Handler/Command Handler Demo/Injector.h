#pragma once

#include "di.h"
#include "Repositories.h"
#include "Handlers.h"
#include "Commands.h"

namespace IoC
{
	namespace di = boost::di;

	static bool initialized = false;

	//auto CreateInjector() -> decltype(di::make_injector())
	//{
	//	if (initialized)
	//		return;

	//	initialized = true;

	//	const auto injector = di::make_injector(
	//		di::bind<IUserRepository>().to<InMemoryUserRepository>(),
	//		di::bind<ICommandHandler<Register>>().to<RegisterHandler>()
	//	);

	//	return injector;
	//}
}

