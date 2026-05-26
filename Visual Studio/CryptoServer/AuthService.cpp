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

    if (user.passwordHash != password) {
        cout << "Invalid password." << endl;
        return nullopt;
    }

    cout << "Login successful." << endl;

    if (user.banned) {
        cout << "User is banned." << endl;
        return nullopt;
    }
    return user;
}

bool AuthService::isAdmin(const string& username) {
    UserRepository repo;
    auto userOpt = repo.findByUsername(username);
    if (!userOpt.has_value()) return false;
    return userOpt.value().role == "admin";
}

vector<User> AuthService::getAllUsers(const string& adminUsername) {
    if (!isAdmin(adminUsername)) {
        cout << "Access denied: not admin." << endl;
        return {};
    }
    UserRepository repo;
    return repo.getAllUsers();
}

bool AuthService::deleteUser(const string& adminUsername, const string& targetUsername) {
    if (!isAdmin(adminUsername)) {
        cout << "Access denied: not admin." << endl;
        return false;
    }
    if (adminUsername == targetUsername) {
        cout << "Cannot delete yourself." << endl;
        return false;
    }
    UserRepository repo;
    return repo.deleteUser(targetUsername);
}

bool AuthService::updateUserRole(const string& adminUsername, const string& targetUsername, const string& newRole) {
    if (!isAdmin(adminUsername)) {
        cout << "Access denied: not admin." << endl;
        return false;
    }
    UserRepository repo;
    return repo.updateUserRole(targetUsername, newRole);
}

bool AuthService::isBanned(const string& username) {
    UserRepository repo;
    auto userOpt = repo.findByUsername(username);
    if (!userOpt.has_value()) return false;
    return userOpt.value().banned;
}

bool AuthService::banUser(const string& adminUsername, const string& targetUsername) {
    if (!isAdmin(adminUsername)) {
        cout << "Access denied: not admin." << endl;
        return false;
    }
    if (adminUsername == targetUsername) {
        cout << "Cannot ban yourself." << endl;
        return false;
    }
    UserRepository repo;
    return repo.banUser(targetUsername);
}

bool AuthService::unbanUser(const string& adminUsername, const string& targetUsername) {
    if (!isAdmin(adminUsername)) {
        cout << "Access denied: not admin." << endl;
        return false;
    }
    UserRepository repo;
    return repo.unbanUser(targetUsername);
}