#pragma once

#include "User.h"
#include <vector>

struct IRepository
{
	// Marker interface
	virtual ~IRepository() noexcept = default;
};

struct IUserRepository : IRepository
{
	virtual void AddUser(const User& user) = 0;
	virtual User& FindById(const UserId& id) = 0;
	virtual std::vector<User>& Browse() = 0;
	virtual void Delete(const UserId& id) = 0;
	virtual ~IUserRepository() noexcept = default;
};

class InMemoryUserRepository : public IUserRepository
{
public:
	void AddUser(const User& user) override;
	User& FindById(const UserId& id) override;
	std::vector<User>& Browse() override;
	void Delete(const UserId& id) override;

private:
	std::vector<User> users_;
};