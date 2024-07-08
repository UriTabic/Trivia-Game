#pragma once
#include <string>
using std::string;

/*
* A class that describes a logged user in the server.
*/
class LoggedUser
{
public:
	/*
	* Generates instance with given username
	*/
	LoggedUser(const string& username);
	~LoggedUser() = default;
	LoggedUser() = default;
	/*
	* @returns the username
	*/
	string getUsername() const;

	/*
	* @return whether the names are the same
	*/
	bool operator==(const LoggedUser& other);
	/*
	* @return whether the first name is less than the second
	*/
	friend bool operator<(const LoggedUser& user, const LoggedUser& other);
private:
	string mUsername;
};