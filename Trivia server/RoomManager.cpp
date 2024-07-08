#include "RoomManager.h"
#include "SqliteDataBase.h"

RoomManager* RoomManager::instancePtr = nullptr;

RoomManager* RoomManager::getInstance()
{
	if (instancePtr == nullptr)
	{
		instancePtr = new RoomManager();
	}
	return instancePtr;
}

RoomManager::RoomManager() 
{
	mId = SqliteDataBase::getInstance()->getNextId();
}

void RoomManager::createRoom(const LoggedUser& user, const RoomData& roomData)
{
	Room room(roomData);
	room.addUser(user);
	mRooms.insert({roomData.id, room});
}

void RoomManager::deleteRoom(const unsigned int id)
{
	mRooms.erase(id);
}

unsigned int RoomManager::getRoomState(const unsigned int id)
{
	return mRooms[id].getRoomData().state;
}

vector<RoomData> RoomManager::getRooms()
{
	vector<RoomData> rooms;

	for (auto room : mRooms)
	{
		rooms.push_back(room.second.getRoomData());
	}

	return rooms;
}

Room& RoomManager::getRoom(const unsigned int id)
{
	return mRooms[id];
}

int RoomManager::getNextId()
{
	return mId++;
}

RoomManager::~RoomManager()
{
	delete instancePtr;
	instancePtr = nullptr;
}
