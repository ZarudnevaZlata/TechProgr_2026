#pragma once

#include "User.h"
#include <optional>
#include <string>

using namespace std;

class AuthService {
public:
    bool registerUser(const string& username, const string& password);
    optional<User> loginUser(const string& username, const string& password);
};