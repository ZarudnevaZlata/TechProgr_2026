<<<<<<< Updated upstream
#include "AlgorithmService.h"

using namespace std;

string AlgorithmService::vigenereEncrypt(const string& text, const string& key) {
    if (key.empty()) return "error: empty key";
    
    string result = text;
    size_t keyLen = key.length();
    
    for (size_t i = 0; i < text.length(); i++) {
        result[i] = text[i] + key[i % keyLen];
    }
    return result;
}

string AlgorithmService::vigenereDecrypt(const string& text, const string& key) {
    if (key.empty()) return "error: empty key";
    
    string result = text;
    size_t keyLen = key.length();
    
    for (size_t i = 0; i < text.length(); i++) {
        result[i] = text[i] - key[i % keyLen];
    }
    return result;
}

string AlgorithmService::sha1Stub() {
    return "SHA1 is not implemented yet.";
}

string AlgorithmService::newtonStub() {
    return "Newton method is not implemented yet.";
}

string AlgorithmService::audioEmbedStub() {
    return "Audio embed is not implemented yet.";
}

string AlgorithmService::audioExtractStub() {
    return "Audio extract is not implemented yet.";
}
=======
#include "AlgorithmService.h"
#include "ExpressionParser.h"
#include "AudioSteganography.h"
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

string AlgorithmService::newtonSolve(const string& expression, double x0) {
    const double epsilon = 1e-7;
    const int maxIterations = 100;

    double x = x0;
    int iteration = 0;

    try {
        for (iteration = 0; iteration < maxIterations; ++iteration) {
            double fx = ExpressionParser::evaluate(expression, x);

            if (abs(fx) < epsilon) {
                ostringstream oss;
                oss << fixed << setprecision(10);
                oss << "{\"root\":" << x
                    << ",\"iterations\":" << iteration
                    << ",\"f(x)\":" << fx << "}";
                return oss.str();
            }

            double fpx = ExpressionParser::derivative(expression, x);

            if (abs(fpx) < 1e-12) {
                return "{\"error\":\"Derivative is zero, cannot continue.\"}";
            }

            double xNew = x - fx / fpx;

            if (abs(xNew - x) < epsilon) {
                ostringstream oss;
                oss << fixed << setprecision(10);
                oss << "{\"root\":" << xNew
                    << ",\"iterations\":" << (iteration + 1)
                    << ",\"f(x)\":" << ExpressionParser::evaluate(expression, xNew) << "}";
                return oss.str();
            }

            x = xNew;
        }

        ostringstream oss;
        oss << fixed << setprecision(10);
        oss << "{\"error\":\"Did not converge after " << maxIterations
            << " iterations\",\"last_x\":" << x << "}";
        return oss.str();
    }
    catch (const exception& e) {
        return "{\"error\":\"" + string(e.what()) + "\"}";
    }
}

vector<uint8_t> AlgorithmService::audioEmbed(const vector<uint8_t>& wavData, const string& message) {
    return AudioSteganography::embedMessage(wavData, message);
}

string AlgorithmService::audioExtract(const vector<uint8_t>& wavData) {
    return AudioSteganography::extractMessage(wavData);
}

static bool isEnglish(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static bool isRussian(char c) {
    // Русские буквы в cp1251 (консоль Windows)
    return (c >= 'А' && c <= 'я') || c == 'Ё' || c == 'ё';
}

string AlgorithmService::vigenereEncrypt(const string& text, const string& key) {
    if (key.empty()) return text;

    string result;
    size_t keyIndex = 0;

    for (char c : text) {
        if (isEnglish(c)) {
            bool isUpper = isupper(c);
            char base = isUpper ? 'A' : 'a';
            char keyChar = tolower(key[keyIndex % key.size()]) - 'a';
            char encrypted = (c - base + keyChar) % 26 + base;
            result += encrypted;
            keyIndex++;
        }
        else if (isRussian(c)) {
            bool isUpper = isupper(c);
            char base = isUpper ? 'А' : 'а';
            char keyChar = tolower(key[keyIndex % key.size()]) - 'a';
            char encrypted = (c - base + keyChar) % 32 + base;
            result += encrypted;
            keyIndex++;
        }
        else {
            // Цифры, пробелы, знаки — без изменений
            result += c;
        }
    }

    return result;
}

string AlgorithmService::vigenereDecrypt(const string& text, const string& key) {
    if (key.empty()) return text;

    string result;
    size_t keyIndex = 0;

    for (char c : text) {
        if (isEnglish(c)) {
            bool isUpper = isupper(c);
            char base = isUpper ? 'A' : 'a';
            char keyChar = tolower(key[keyIndex % key.size()]) - 'a';
            char decrypted = (c - base - keyChar + 26) % 26 + base;
            result += decrypted;
            keyIndex++;
        }
        else if (isRussian(c)) {
            bool isUpper = isupper(c);
            char base = isUpper ? 'А' : 'а';
            char keyChar = tolower(key[keyIndex % key.size()]) - 'a';
            char decrypted = (c - base - keyChar + 32) % 32 + base;
            result += decrypted;
            keyIndex++;
        }
        else {
            result += c;
        }
    }

    return result;
}

string AlgorithmService::sha1Stub() {
    return "SHA1 is not implemented yet.";
}
>>>>>>> Stashed changes
