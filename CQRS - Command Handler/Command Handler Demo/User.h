#pragma once

#include <iostream>
#include <atomic>

namespace Zuravvski
{
	namespace Demo
	{
		class UserId
		{
		public:
			static UserId CreateUnique();

			friend bool operator==(const UserId& first, const UserId& second);
			friend bool operator!=(const UserId& first, const UserId& second);
			friend std::ostream& operator<<(std::ostream& outputStream, const UserId& userId);

		protected:
			UserId();

		private:
			int id_;
			static std::atomic<int> qttyCounter_;
		};

		class User
		{
		public:
			template<typename TString, typename = std::enable_if_t<std::is_assignable<std::string, TString>::value>>
			static User Create(TString&& login, TString&& password, TString&& salt)
			{
				return User(std::forward<TString>(login), std::forward<TString>(password), std::forward<TString>(salt));
			}

			std::string GetName() const;
			std::string GetPassword() const;
			UserId GetId() const;

		protected:
			template<typename TString, typename = std::enable_if_t<std::is_assignable<std::string, TString>::value>>
			User(TString&& login, TString&& password, TString&& salt) :
				id_(UserId::CreateUnique()),
				login_(std::forward<TString>(login)),
				password_(std::forward<TString>(password)),
				salt_(std::forward<TString>(salt)) {}

		private:
			UserId id_;
			std::string login_;
			std::string password_;
			std::string salt_;
		};

		using UserPtr = std::shared_ptr<User>;
	}
}