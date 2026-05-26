#pragma once

#include <string>

using namespace std;

class AlgorithmService {
public:
    string vigenereEncrypt(const string& text, const string& key);
    string vigenereDecrypt(const string& text, const string& key);
    string sha1Stub();
    string newtonStub();
    string audioEmbedStub();
    string audioExtractStub();
};
