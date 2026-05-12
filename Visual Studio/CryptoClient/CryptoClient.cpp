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
    double x0 = 0.0;

    while (true) {
        cout << endl;
        cout << "1 - Ping server" << endl;
        cout << "2 - Register" << endl;
        cout << "3 - Login" << endl;
        cout << "4 - Vigenere encrypt" << endl;
        cout << "5 - Vigenere decrypt" << endl;
        cout << "6 - SHA1" << endl;
        cout << "7 - Newton method" << endl;
        cout << "8 - Audio embed" << endl;
        cout << "9 - Audio extract" << endl;
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
            api.loginUser(username, password);
            break;
        case 4:
            cout << "Username: "; cin >> username;
            cout << "Text: "; cin >> text;
            cout << "Key: "; cin >> key;
            api.vigenereEncrypt(username, text, key);
            break;
        case 5:
            cout << "Username: "; cin >> username;
            cout << "Text: "; cin >> text;
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
        default:
            cout << "Invalid choice." << endl;
        }
    }
    return 0;
}