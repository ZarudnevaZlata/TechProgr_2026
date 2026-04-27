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
            "SELECT id, username, password_hash, role, created_at FROM users WHERE username = $1",
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