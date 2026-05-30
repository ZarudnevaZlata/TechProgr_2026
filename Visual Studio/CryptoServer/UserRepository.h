#pragma once

#include "User.h"
#include <optional>
#include <string>
#include <vector>

using namespace std;

class UserRepository {
public:
    bool createUser(const string& username, const string& passwordHash, const string& role = "user");
    optional<User> findByUsername(const string& username);
    bool usernameExists(const string& username);
    vector<User> getAllUsers();
    bool deleteUser(const string& username);
    bool updateUserRole(const string& username, const string& newRole);
    bool banUser(const string& username);
    bool unbanUser(const string& username);
};