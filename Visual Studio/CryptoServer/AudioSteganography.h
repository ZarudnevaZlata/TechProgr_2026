#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <cstdint>

using namespace std;

class AudioSteganography {
public:
    static vector<uint8_t> embedMessage(const vector<uint8_t>& wavData, const string& message) {
        if (wavData.size() < 44) return {};

        if (wavData[0] != 'R' || wavData[1] != 'I' || wavData[2] != 'F' || wavData[3] != 'F') {
            return {};
        }

        vector<uint8_t> result = wavData;
        vector<uint8_t> audioData(result.begin() + 44, result.end());

        uint32_t msgLen = (uint32_t)message.size();

        // Пишем длину: 4 байта = 32 бита, big-endian
        vector<uint8_t> messageBits;
        for (int i = 31; i >= 0; --i) {
            messageBits.push_back((msgLen >> i) & 1);
        }
        // Пишем сообщение: каждый байт big-endian по битам
        for (char c : message) {
            for (int bit = 7; bit >= 0; --bit) {
                messageBits.push_back((c >> bit) & 1);
            }
        }

        if (messageBits.size() > audioData.size()) return {};

        for (size_t i = 0; i < messageBits.size(); ++i) {
            audioData[i] = (audioData[i] & 0xFE) | messageBits[i];
        }

        copy(audioData.begin(), audioData.end(), result.begin() + 44);
        return result;
    }

    static string extractMessage(const vector<uint8_t>& wavData) {
        if (wavData.size() < 48) {
            return "{\"error\":\"File too small.\"}";
        }

        if (wavData[0] != 'R' || wavData[1] != 'I' || wavData[2] != 'F' || wavData[3] != 'F') {
            return "{\"error\":\"Not a WAV file.\"}";
        }

        const uint8_t* audio = wavData.data() + 44;
        size_t audioSize = wavData.size() - 44;

        if (audioSize < 32) {
            return "{\"error\":\"No hidden data.\"}";
        }

        // Читаем длину: 32 бита, big-endian
        uint32_t msgLen = 0;
        for (int i = 0; i < 32; ++i) {
            msgLen = (msgLen << 1) | (audio[i] & 1);
        }

        if (msgLen == 0) {
            return "{\"message\":\"\",\"message_length\":0}";
        }

        if (msgLen > 100000) {
            return "{\"error\":\"No message found, invalid length.\"}";
        }

        size_t totalBits = 32 + msgLen * 8;
        if (totalBits > audioSize) {
            return "{\"error\":\"File too small for message.\"}";
        }

        // Читаем сообщение, big-endian
        string message;
        for (uint32_t byteIdx = 0; byteIdx < msgLen; ++byteIdx) {
            char c = 0;
            for (int bit = 0; bit < 8; ++bit) {
                size_t pos = 32 + byteIdx * 8 + bit;
                c = (c << 1) | (audio[pos] & 1);
            }
            message += c;
        }

        // Экранируем
        string escaped;
        for (char c : message) {
            switch (c) {
            case '"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default:
                if (c >= 32 && c <= 126) escaped += c;
                else escaped += "?";
            }
        }

        return "{\"message\":\"" + escaped + "\",\"message_length\":" + to_string(msgLen) + "}";
    }
};