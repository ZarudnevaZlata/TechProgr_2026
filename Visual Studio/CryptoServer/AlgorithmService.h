#pragma once

#include <string>
#include <vector>
#include <cstdint>

using namespace std;

class AlgorithmService {
public:
    string vigenereEncrypt(const string& text, const string& key);
    string vigenereDecrypt(const string& text, const string& key);
    string sha1Stub();
    string newtonSolve(const string& expression, double x0);
    vector<uint8_t> audioEmbed(const vector<uint8_t>& wavData, const string& message);
    string audioExtract(const vector<uint8_t>& wavData);
};
