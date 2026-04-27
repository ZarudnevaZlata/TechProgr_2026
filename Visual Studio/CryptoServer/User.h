#pragma once

#include <string>

using namespace std;

struct User {
    int id = 0;
    string username;
    string passwordHash;
    string role;
    string createdAt;
};