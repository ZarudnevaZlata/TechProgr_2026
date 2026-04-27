#include "AuthService.h"
#include "UserRepository.h"
#include <iostream>

using namespace std;

bool AuthService::registerUser(const string& username, const string& password) {
    UserRepository repo;

    if (repo.usernameExists(username)) {
        cout << "User already exists." << endl;
        return false;
    }

    // Временный вариант: без хеширования
    string passwordHash = password;

    bool ok = repo.createUser(username, passwordHash, "user");

    if (ok) {
        cout << "Registration successful." << endl;
    }
    else {
        cout << "Registration failed." << endl;
    }

    return ok;
}

optional<User> AuthService::loginUser(const string& username, const string& password) {
    UserRepository repo;
    auto userOpt = repo.findByUsername(username);

    if (!userOpt.has_value()) {
        cout << "User not found." << endl;
        return nullopt;
    }

    User user = userOpt.value();

    // Временный вариант: прямое сравнение
    if (user.passwordHash != password) {
        cout << "Invalid password." << endl;
        return nullopt;
    }

    cout << "Login successful." << endl;
    return user;
}