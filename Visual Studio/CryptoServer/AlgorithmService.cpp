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
