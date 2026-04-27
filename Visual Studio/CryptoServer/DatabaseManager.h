#pragma once

#include <memory>
#include <string>
#include <pqxx/pqxx>

using namespace std;

class DatabaseManager {
private:
    static DatabaseManager* instance;
    unique_ptr<pqxx::connection> connection;

    DatabaseManager();

public:
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    static DatabaseManager& getInstance();

    bool connect();
    bool isConnected() const;
    pqxx::connection& getConnection();
};