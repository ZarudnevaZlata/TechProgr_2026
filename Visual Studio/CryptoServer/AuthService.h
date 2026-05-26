#pragma once

#include "User.h"
#include <optional>
#include <string>
#include <vector>

using namespace std;

class AuthService {
public:
    bool registerUser(const string& username, const string& password);
    optional<User> loginUser(const string& username, const string& password);
    bool isAdmin(const string& username);
    vector<User> getAllUsers(const string& adminUsername);
    bool deleteUser(const string& adminUsername, const string& targetUsername);
    bool updateUserRole(const string& adminUsername, const string& targetUsername, const string& newRole);
    bool banUser(const string& adminUsername, const string& targetUsername);
    bool unbanUser(const string& adminUsername, const string& targetUsername);
    bool isBanned(const string& username);
};