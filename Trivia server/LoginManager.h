#pragma once

#include "LoggedUser.h"
#include "IDatabase.h"
#include <vector>

using std::vector;

enum RESULTS
{
    VALID = 0,
    MISMATCH = 1,
    LOGGED = 2,
    WEAK_PASSWORD = 3,
    SHORT_USERNAME = 4,
    ILLEGAL_EMAIL = 5
};


/****
 * @brief The LoginManager class handles user login, signup, and logout operations.
 *
 * This singleton class manages user authentication, including user registration, login,
 * and logout functionality. It interacts with the database to verify and store user data.
 ****/
class LoginManager
{
public:
    /****
     * @brief Deleted copy constructor to enforce singleton pattern.
     *
     * This prevents copying of the singleton instance.
     *
     * @param obj A reference to another LoginManager object.
     ****/
    LoginManager(const LoginManager& obj) = delete;

    /****
     * @brief Gets the singleton instance of LoginManager.
     *
     * This method ensures that only one instance of LoginManager exists.
     *
     * @returns A pointer to the singleton instance of LoginManager.
     ****/
    static LoginManager* getInstance();

    /****
     * @brief Registers a new user.
     *
     * This method adds a new user to the database and logs them in if successful.
     *
     * @param username The username of the new user.
     * @param password The password of the new user.
     * @param email The email of the new user.
     * @returns True if the user was successfully registered, otherwise false.
     ****/
    bool signup(const string& username, const string& password, const string& email);

    /****
     * @brief Logs in an existing user.
     *
     * This method checks the user's credentials and logs them in if they match.
     *
     * @param username The username of the user.
     * @param password The password of the user.
     * @returns 'v' if login is successful, 'm' if the password does not match, 'e' if the user is already logged in.
     ****/
    RESULTS login(const string& username, const string& password);

    /****
     * @brief Logs out an existing user.
     *
     * This method logs out the user by removing them from the logged-in users list.
     *
     * @param username The username of the user to be logged out.
     ****/
    void logout(const string& username);

private:
    vector<LoggedUser> mLoggedUsers; ///< List of currently logged-in users.
    static IDataBase* mDb;           ///< Pointer to the database instance.
    static LoginManager* instancePtr;///< Pointer to the singleton instance.

    /****
     * @brief Private constructor to enforce singleton pattern.
     *
     * This constructor initializes necessary members.
     ****/
    LoginManager() = default;

    /****
     * @brief Destructor for LoginManager.
     *
     * This destructor cleans up the singleton instance.
     ****/
    ~LoginManager();
};
