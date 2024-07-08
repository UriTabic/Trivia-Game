#include "LoginManager.h"
#include <algorithm>
#include "SqliteDataBase.h"
LoginManager* LoginManager::instancePtr = nullptr;
IDataBase* LoginManager::mDb = SqliteDataBase::getInstance();

LoginManager* LoginManager::getInstance()
{
    if (instancePtr == nullptr)
    {
        instancePtr = new LoginManager();
    }
    return instancePtr;
}

bool LoginManager::signup(const string& username, const string& password, const string& email)
{
    if (mDb->doesUserExists(username))
        return false;

    bool res = mDb->addUser(username, password, email);
    if (res)
    {
        LoggedUser user(username);
        mLoggedUsers.push_back(user);
    }
    return res;
}

RESULTS LoginManager::login(const string& username, const string& password)
{
    if (mDb->doesPasswordMatch(username, password))
    {
        if (std::find_if(mLoggedUsers.begin(), mLoggedUsers.end(), [&](const LoggedUser& obj) {
            return obj.getUsername() == username;
            }) != mLoggedUsers.end()
                )
        {
            return RESULTS::LOGGED;
        }
        LoggedUser user(username);
        mLoggedUsers.push_back(user);
        return RESULTS::VALID;
    }
    return RESULTS::MISMATCH;
}

void LoginManager::logout(const string& username)
{
    mLoggedUsers.erase(
        std::remove_if(
            mLoggedUsers.begin(), 
            mLoggedUsers.end(), 
            [&](const LoggedUser& obj) 
            {
                return obj.getUsername() == username;
            }
        ),
        mLoggedUsers.end());
}

LoginManager::~LoginManager()
{
    delete instancePtr;
}
