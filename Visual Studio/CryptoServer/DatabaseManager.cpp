#include "DatabaseManager.h"
#include <iostream>

using namespace std;

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() {
}

DatabaseManager& DatabaseManager::getInstance() {
    if (instance == nullptr) {
        instance = new DatabaseManager();
    }
    return *instance;
}

bool DatabaseManager::connect() {
    try {
        if (connection && connection->is_open()) {
            return true;
        }

        string connStr =
            "host=localhost "
            "port=5432 "
            "dbname=crypto_db "
            "user=crypto_user "
            "password=crypto_pass";

        connection = make_unique<pqxx::connection>(connStr);

        if (connection->is_open()) {
            cout << "Connected to database: " << connection->dbname() << endl;
            return true;
        }

        cout << "Failed to open database connection." << endl;
        return false;
    }
    catch (const exception& e) {
        cout << "Database connection error: " << e.what() << endl;
        return false;
    }
}

bool DatabaseManager::isConnected() const {
    return connection && connection->is_open();
}

pqxx::connection& DatabaseManager::getConnection() {
    return *connection;
}