#include "Repositories.h"
#include <algorithm>

void InMemoryUserRepository::AddUser(const User & user)
{
	users_.emplace_back(user);
}

User & InMemoryUserRepository::FindById(const UserId & id)
{
	auto foundUser = std::find_if(users_.begin(), users_.end(), [&id](const User& user)
	{
		return id == user.GetId();
	});

	if (foundUser == users_.end())
	{
		throw std::runtime_error{ "Could not find the user with a given ID" };
	}

	return *foundUser;
}

std::vector<User>& InMemoryUserRepository::Browse()
{
	return users_;
}

void InMemoryUserRepository::Delete(const UserId & id)
{
	auto foundUser = std::find_if(users_.begin(), users_.end(), [&id](const User& user)
	{
		return id == user.GetId();
	});

	if (foundUser != users_.end())
	{
		users_.erase(foundUser);
	}
}




