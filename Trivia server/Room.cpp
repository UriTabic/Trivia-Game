#include "Room.h"

Room::Room()
{
	mMetaData = {0, "", 0, 0, 0, 0};
}

Room::Room(const RoomData& MetaData)
{
	mMetaData = MetaData;
}

bool Room::addUser(const LoggedUser& user)
{
	for (auto it = mUsers.begin(); it != mUsers.end(); ++it)
	{
		if (it->getUsername() == user.getUsername())
		{
			return false;
		}
	}
	mUsers.push_back(user);
	return true;
}

void Room::removeUser(const LoggedUser& user)
{
	mUsers.erase(std::find(mUsers.begin(), mUsers.end(), user));
}

vector<string> Room::getAllUsers() const
{
	vector<string> users;

	for (auto user : mUsers)
	{
		users.push_back(user.getUsername());
	}

	return users;
}

RoomData& Room::getRoomData()
{
	return mMetaData;
}

void Room::setState(const RoomState state)
{
	mMetaData.state = state;
}
