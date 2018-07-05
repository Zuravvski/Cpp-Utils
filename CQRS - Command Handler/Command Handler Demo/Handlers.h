#pragma once

#include "Commands.h"
#include "Repositories.h"
#include <future>
#include <memory>

namespace Zuravvski
{
	namespace Demo
	{
		template<typename T, typename = std::enable_if_t<std::is_base_of<ICommand, std::decay_t<T>>::value>>
		struct ICommandHandler
		{
			virtual std::future<void> Handle(std::unique_ptr<T> command) = 0;
			virtual ~ICommandHandler() noexcept = default;
		};

		class RegisterHandler : public ICommandHandler<Register>
		{
		public:
			RegisterHandler(std::shared_ptr<IUserRepository> userRepository) :
				userRepository_(userRepository)
			{
			}

			std::future<void> Handle(std::unique_ptr<Register> command) override
			{
				return std::async(std::launch::async, [command = std::move(command), this]
					{
						if (command->login.empty() || command->password.empty())
						{
							throw std::runtime_error{ "The login and the password cannot be empty" };
						}

				std::cout << "Login: " << command->login << "\n"
					<< "Password: " << command->password
					<< std::endl;

				// TODO: Should be copiable and moveable
				userRepository_->AddUser(User::Create(command->login, command->password, command->salt));
					});
			}

		private:
			std::shared_ptr<IUserRepository> userRepository_;
		};

		class BrowseUsersHandler : public ICommandHandler<BrowseUsers>
		{
		public:
			BrowseUsersHandler(std::shared_ptr<IUserRepository> userRepository) :
				userRepository_(userRepository)
			{
			}

			std::future<void> Handle(std::unique_ptr<BrowseUsers> command) override
			{
				return std::async(std::launch::async, [command = std::move(command), this]
					{
						std::vector<User>& users = userRepository_->Browse();
				for (auto& user : users)
				{
					std::cout << "ID: " << user.GetId()
						<< "\tName: " << user.GetName()
						<< "\tPassword: " << user.GetPassword()
						<< std::endl;
				}
					});
			}

		private:
			std::shared_ptr<IUserRepository> userRepository_;
		};
	}
}