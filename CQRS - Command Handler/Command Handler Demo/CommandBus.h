#pragma once

#include <future>
#include <memory>
#include <Hypodermic/ComponentContext.h>
#include "Handlers.h"

namespace Zuravvski 
{
	namespace Demo
	{
		struct ICommandBus
		{
			// TODO: To be implemented
			// Type erasure is probably needed to marry template and virtual method
			// Maybe CRTP?
			virtual ~ICommandBus() noexcept = default;
		};

		class CommandBus : public ICommandBus
		{
		public:
			CommandBus(std::shared_ptr<Hypodermic::Container> componentContext) :
				componentContext_(componentContext)
			{
			}

			template<typename T, typename = std::enable_if_t<std::is_base_of<ICommand, std::decay_t<T>>::value>>
			std::future<void> Dispatch(std::unique_ptr<T> command)
			{
				if (!command)
				{
					throw std::runtime_error{ "The command is nullptr" };
				}

				return std::async(std::launch::async, [command = std::move(command), this]() mutable
				{
					const auto commandName = typeid(*command).name();

					try
					{
						const auto handler = componentContext_->resolve<ICommandHandler<T>>();

						if (handler)
						{
							handler->Handle(std::move(command)).get();
						}

					}
					catch (std::exception& e)
					{
						std::cerr << "Error has occured while executing the command " << commandName << std::endl;
						std::cerr << "Message: " << e.what() << std::endl;
					}
				});
			}

		private:
			std::shared_ptr<Hypodermic::Container> componentContext_;
		};
	}
}