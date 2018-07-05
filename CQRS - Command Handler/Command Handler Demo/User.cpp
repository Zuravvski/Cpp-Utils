#include "User.h"

std::string User::GetName() const
{
	return login_;
}

std::string User::GetPassword() const
{
	return password_;
}

UserId User::GetId() const
{
	return id_;
}

// ------------------------------- UserId ----------------------------//

std::atomic<int> UserId::qttyCounter_{ 0 };

UserId UserId::CreateUnique()
{
	return UserId();
}

UserId::UserId()
{
	id_ = ++qttyCounter_;
}

bool operator==(const UserId & first, const UserId & second)
{
	return first.id_ == second.id_;
}

bool operator!=(const UserId & first, const UserId & second)
{
	return operator==(first, second);
}

std::ostream & operator<<(std::ostream & outputStream, const UserId & userId)
{
	outputStream << userId.id_;
	return outputStream;
}
