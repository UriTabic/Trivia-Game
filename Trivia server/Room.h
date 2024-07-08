#pragma once

#include "LoggedUser.h"
#include <iostream>
#include <vector>

using std::string;
using std::vector;

/*
* Describes the current state of the room
*/
enum RoomState{OPENED = 0, CLOSED, STARTED};

/*
* Describes the specification of a room.
*/
struct RoomData
{
	unsigned int id;
	string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int state;
};

/*
* A class that represent a room, its users and its data
*/
class Room
{
public:
	/*
	* Generates an empty room with default values
	*/
	Room();
	/*
	* Generates a room with the given data and no users.
	* @param MetaData - the room specs
	*/
	Room(const RoomData& MetaData);
	~Room() = default;
	/*
	* Adds a user to the room
	* @param user - the user to add.
	* @returns is seccessful.
	*/
	bool addUser(const LoggedUser& user);
	/*
	* Removes a user from the room
	* @param user - the user to remove
	* @returns is seccessful.
	*/
	void removeUser(const LoggedUser& user);
	/*
	* @returns all the users in the room
	*/
	vector<string> getAllUsers() const;
	/*
	* @returns the room specs
	*/
	RoomData& getRoomData();

	/*
	* @param state - the new state.
	* 
	* @returns None.
	*/
	void setState(const RoomState state);

private:
	RoomData mMetaData;
	vector<LoggedUser> mUsers;
};