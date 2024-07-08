#include "LoggedUser.h"

LoggedUser::LoggedUser(const string& username)
{
	mUsername = username;
}

string LoggedUser::getUsername() const
{
	return mUsername;
}

bool LoggedUser::operator==(const LoggedUser& other)
{
	return other.mUsername == mUsername;
}

bool operator<(const LoggedUser& user, const LoggedUser& other)
{
	return user.getUsername() < other.getUsername();
}

