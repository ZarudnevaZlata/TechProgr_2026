#include <iostream>
#include "ApiClient.h"

using namespace std;

int main() {
    ApiClient& api = ApiClient::getInstance();

    int choice;
    string username;
    string password;
    string text;
    string key;
    string expression;
    string filePath;
    string message;
    string currentUser = "";
    string currentRole = "";
    double x0 = 0.0;

    while (true) {
        cout << endl;
        if (!currentUser.empty()) {
            cout << "Logged in as: " << currentUser << " (" << currentRole << ")" << endl;
        }
        cout << "1 - Ping server" << endl;
        cout << "2 - Register" << endl;
        cout << "3 - Login" << endl;
        cout << "4 - Vigenere encrypt" << endl;
        cout << "5 - Vigenere decrypt" << endl;
        cout << "6 - SHA1" << endl;
        cout << "7 - Newton method" << endl;
        cout << "8 - Audio embed" << endl;
        cout << "9 - Audio extract" << endl;
        if (currentRole == "admin") {
            cout << "10 - Admin: List users" << endl;
            cout << "11 - Admin: Delete user" << endl;
            cout << "12 - Admin: Change user role" << endl;
            cout << "13 - Admin: Ban user" << endl;
            cout << "14 - Admin: Unban user" << endl;
        }
        cout << "0 - Exit" << endl;
        cout << "Choose: ";
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
        case 1: api.ping(); break;
        case 2:
            cout << "Username: "; cin >> username;
            cout << "Password: "; cin >> password;
            api.registerUser(username, password);
            break;
        case 3:
            cout << "Username: "; cin >> username;
            cout << "Password: "; cin >> password;
            {
                // Отправляем логин и смотрим на результат
                // api.loginUser возвращает bool — успех или нет
                bool ok = api.loginUser(username, password);
                if (!ok) {
                    // Логин не удался — сбрасываем currentUser
                    currentUser = "";
                    currentRole = "";
                    break;
                }
                // Логин удался — спрашиваем роль
                cout << "Are you admin? (y/n): ";
                char isAdmin;
                cin >> isAdmin;
                if (isAdmin == 'y' || isAdmin == 'Y') {
                    currentUser = username;
                    currentRole = "admin";
                }
                else {
                    currentUser = username;
                    currentRole = "user";
                }
            }
            break;
        case 4:
            cout << "Username: "; cin >> username;
            cin.ignore();
            cout << "Text: "; getline(cin, text);
            cout << "Key: "; cin >> key;
            api.vigenereEncrypt(username, text, key);
            break;

        case 5:
            cout << "Username: "; cin >> username;
            cin.ignore();
            cout << "Text: "; getline(cin, text);
            cout << "Key: "; cin >> key;
            api.vigenereDecrypt(username, text, key);
            break;
        case 6:
            cout << "Username: "; cin >> username;
            cout << "Text: "; cin >> text;
            api.sha1Hash(username, text);
            break;
        case 7:
            cout << "Username: "; cin >> username;
            cin.ignore();
            cout << "Expression: "; getline(cin, expression);
            cout << "x0: "; cin >> x0;
            api.newtonSolve(username, expression, x0);
            break;
        case 8:
            cout << "Username: "; cin >> username;
            cin.ignore();
            cout << "File path: "; getline(cin, filePath);
            cout << "Message to hide: "; getline(cin, message);
            api.audioEmbed(username, filePath, message);
            break;
        case 9:
            cout << "Username: "; cin >> username;
            cin.ignore();
            cout << "File path: "; getline(cin, filePath);
            api.audioExtract(username, filePath);
            break;
        case 10:
            if (currentRole != "admin") {
                cout << "Access denied." << endl;
                break;
            }
            api.adminGetUsers(currentUser);
            break;
        case 11:
            if (currentRole != "admin") {
                cout << "Access denied." << endl;
                break;
            }
            {
                string target;
                cout << "Username to delete: "; cin >> target;
                api.adminDeleteUser(currentUser, target);
            }
            break;
        case 12:
            if (currentRole != "admin") {
                cout << "Access denied." << endl;
                break;
            }
            {
                string target, newRole;
                cout << "Username: "; cin >> target;
                cout << "New role (admin/user): "; cin >> newRole;
                api.adminPromote(currentUser, target, newRole);
            }
            break;
        case 13:
            if (currentRole != "admin") {
                cout << "Access denied." << endl;
                break;
            }
            {
                string target;
                cout << "Username to ban: "; cin >> target;
                api.adminBanUser(currentUser, target);
            }
            break;
        case 14:
            if (currentRole != "admin") {
                cout << "Access denied." << endl;
                break;
            }
            {
                string target;
                cout << "Username to unban: "; cin >> target;
                api.adminUnbanUser(currentUser, target);
            }
            break;
        default:
            cout << "Invalid choice." << endl;
        }
    }
    return 0;
}