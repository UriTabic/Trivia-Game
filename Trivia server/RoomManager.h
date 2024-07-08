#pragma once

#include "LoggedUser.h"
#include "Room.h"
#include <map>

using std::map;

/****
 * @brief The RoomManager class manages the creation, deletion, and retrieval of game rooms.
 *
 * This singleton class handles room operations such as creating, deleting,
 * getting room states, and managing the list of rooms.
 ****/
class RoomManager
{
public:
    /****
     * @brief Deleted copy constructor to enforce singleton pattern.
     *
     * This prevents copying of the singleton instance.
     *
     * @param obj A reference to another RoomManager object.
     ****/
    RoomManager(const RoomManager& obj) = delete;

    /****
     * @brief Gets the singleton instance of RoomManager.
     *
     * This method ensures that only one instance of RoomManager exists.
     *
     * @returns A pointer to the singleton instance of RoomManager.
     ****/
    static RoomManager* getInstance();

    /****
     * @brief Creates a new room.
     *
     * This method creates a new room with the specified data and adds the user to it.
     *
     * @param user The user who creates the room.
     * @param roomData The data defining the room's properties.
     ****/
    void createRoom(const LoggedUser& user, const RoomData& roomData);

    /****
     * @brief Deletes an existing room.
     *
     * This method deletes a room with the specified ID.
     *
     * @param id The ID of the room to be deleted.
     ****/
    void deleteRoom(const unsigned int id);

    /****
     * @brief Gets the state of a room.
     *
     * This method retrieves the state of the room with the specified ID.
     *
     * @param id The ID of the room.
     * @returns The state of the room.
     ****/
    unsigned int getRoomState(const unsigned int id);

    /****
     * @brief Gets a list of all rooms.
     *
     * This method retrieves the data of all rooms.
     *
     * @returns A vector of RoomData containing data of all rooms.
     ****/
    vector<RoomData> getRooms();

    /****
     * @brief Gets a room by ID.
     *
     * This method retrieves the room with the specified ID.
     *
     * @param id The ID of the room to retrieve.
     * @returns A reference to the room with the specified ID.
     ****/
    Room& getRoom(const unsigned int id);

    /****
     * @brief Gets the next available room ID.
     *
     * This method provides the next available ID for a new room.
     *
     * @returns The next available room ID.
     ****/
    int getNextId();

private:
    map<unsigned int, Room> mRooms; ///< Map of rooms with their IDs as keys.
    static RoomManager* instancePtr; ///< Pointer to the singleton instance.

    /****
     * @brief Private constructor to enforce singleton pattern.
     *
     * This constructor initializes necessary members.
     ****/
    RoomManager();

    /****
     * @brief Destructor for RoomManager.
     *
     * This destructor cleans up the singleton instance.
     ****/
    ~RoomManager();

    int mId; ///< Counter for the next available room ID.
};
