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

string AlgorithmService::vigenereEncryptStub() {
    return "Vigenere encrypt is not implemented yet.";
}

string AlgorithmService::vigenereDecryptStub() {
    return "Vigenere decrypt is not implemented yet.";
}

string AlgorithmService::sha1Stub() {
    return "SHA1 is not implemented yet.";
}