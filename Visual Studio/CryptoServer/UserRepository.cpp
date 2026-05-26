#include "UserRepository.h"
#include "DatabaseManager.h"
#include <pqxx/pqxx>
#include <iostream>

using namespace std;

bool UserRepository::createUser(const string& username, const string& passwordHash, const string& role) {
    try {
        DatabaseManager& db = DatabaseManager::getInstance();
        pqxx::work txn(db.getConnection());

        pqxx::params params;
        params.append(username);
        params.append(passwordHash);
        params.append(role);

        txn.exec(
            "INSERT INTO users (username, password_hash, role) VALUES ($1, $2, $3)",
            params
        );

        txn.commit();
        return true;
    }
    catch (const exception& e) {
        cout << "createUser error: " << e.what() << endl;
        return false;
    }
}

optional<User> UserRepository::findByUsername(const string& username) {
    try {
        DatabaseManager& db = DatabaseManager::getInstance();
        pqxx::work txn(db.getConnection());

        pqxx::params params;
        params.append(username);

        pqxx::result result = txn.exec(
            "SELECT id, username, password_hash, role, created_at, banned FROM users WHERE username = $1",
            params
        );

        txn.commit();

        if (result.empty()) {
            return nullopt;
        }

        const auto& row = result[0];

        User user;
        user.id = row["id"].as<int>();
        user.username = row["username"].as<string>();
        user.passwordHash = row["password_hash"].as<string>();
        user.role = row["role"].as<string>();
        user.createdAt = row["created_at"].c_str();
        user.banned = row["banned"].as<bool>();

        return user;
    }
    catch (const exception& e) {
        cout << "findByUsername error: " << e.what() << endl;
        return nullopt;
    }
}

bool UserRepository::usernameExists(const string& username) {
    try {
        DatabaseManager& db = DatabaseManager::getInstance();
        pqxx::work txn(db.getConnection());

        pqxx::params params;
        params.append(username);

        pqxx::result result = txn.exec(
            "SELECT 1 FROM users WHERE username = $1",
            params
        );

        txn.commit();
        return !result.empty();
    }
    catch (const exception& e) {
        cout << "usernameExists error: " << e.what() << endl;
        return false;
    }
}

vector<User> UserRepository::getAllUsers() {
    vector<User> users;
    try {
        DatabaseManager& db = DatabaseManager::getInstance();
        pqxx::work txn(db.getConnection());

        pqxx::result result = txn.exec(
            "SELECT id, username, role, created_at, banned FROM users ORDER BY id"
        );

        txn.commit();

        for (const auto& row : result) {
            User user;
            user.id = row["id"].as<int>();
            user.username = row["username"].as<string>();
            user.role = row["role"].as<string>();
            user.createdAt = row["created_at"].c_str();
            user.banned = row["banned"].as<bool>();
            users.push_back(user);
        }
    }
    catch (const exception& e) {
        cout << "getAllUsers error: " << e.what() << endl;
    }
    return users;
}

bool UserRepository::deleteUser(const string& username) {
    try {
        DatabaseManager& db = DatabaseManager::getInstance();
        pqxx::work txn(db.getConnection());

        pqxx::params params;
        params.append(username);

        pqxx::result result = txn.exec(
            "DELETE FROM users WHERE username = $1", params
        );

        txn.commit();
        return result.affected_rows() > 0;
    }
    catch (const exception& e) {
        cout << "deleteUser error: " << e.what() << endl;
        return false;
    }
}

bool UserRepository::updateUserRole(const string& username, const string& newRole) {
    try {
        DatabaseManager& db = DatabaseManager::getInstance();
        pqxx::work txn(db.getConnection());

        pqxx::params params;
        params.append(newRole);
        params.append(username);

        pqxx::result result = txn.exec(
            "UPDATE users SET role = $1 WHERE username = $2", params
        );

        txn.commit();
        return result.affected_rows() > 0;
    }
    catch (const exception& e) {
        cout << "updateUserRole error: " << e.what() << endl;
        return false;
    }
}
bool UserRepository::banUser(const string& username) {
    try {
        DatabaseManager& db = DatabaseManager::getInstance();
        pqxx::work txn(db.getConnection());
        pqxx::params params;
        params.append(username);
        pqxx::result result = txn.exec(
            "UPDATE users SET banned = true WHERE username = $1", params
        );
        txn.commit();
        return result.affected_rows() > 0;
    }
    catch (const exception& e) {
        cout << "banUser error: " << e.what() << endl;
        return false;
    }
}

bool UserRepository::unbanUser(const string& username) {
    try {
        DatabaseManager& db = DatabaseManager::getInstance();
        pqxx::work txn(db.getConnection());
        pqxx::params params;
        params.append(username);
        pqxx::result result = txn.exec(
            "UPDATE users SET banned = false WHERE username = $1", params
        );
        txn.commit();
        return result.affected_rows() > 0;
    }
    catch (const exception& e) {
        cout << "unbanUser error: " << e.what() << endl;
        return false;
    }
}