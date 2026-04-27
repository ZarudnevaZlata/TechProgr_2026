#pragma once

#include "User.h"
#include <optional>
#include <string>

using namespace std;

class UserRepository {
public:
    bool createUser(const string& username, const string& passwordHash, const string& role = "user");
    optional<User> findByUsername(const string& username);
    bool usernameExists(const string& username);
};