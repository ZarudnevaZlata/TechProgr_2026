#pragma once

#include <string>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <cctype>

using namespace std;

class ExpressionParser {
public:
    static double evaluate(const string& expression, double x) {
        string expr = removeSpaces(expression);
        string withValue = replaceX(expr, x);
        size_t pos = 0;
        return parseAddSub(withValue, pos);
    }

    static double derivative(const string& expression, double x) {
        const double h = 1e-6;
        double f1 = evaluate(expression, x + h);
        double f2 = evaluate(expression, x - h);
        return (f1 - f2) / (2.0 * h);
    }

private:
    static string removeSpaces(const string& str) {
        string result;
        for (char c : str)
            if (!isspace(c)) result += c;
        return result;
    }

    static string replaceX(const string& expr, double x) {
        string result;
        char buffer[64];
        sprintf_s(buffer, "%.15g", x);
        string xStr(buffer);

        for (size_t i = 0; i < expr.size(); ++i) {
            char c = expr[i];
            if ((c == 'x' || c == 'X') &&
                (i == 0 || !isalpha(expr[i - 1])) &&
                (i + 1 >= expr.size() || !isalpha(expr[i + 1]))) {
                result += "(" + xStr + ")";
            }
            else {
                result += c;
            }
        }
        return result;
    }

    static double parseAddSub(const string& expr, size_t& pos) {
        double left = parseMulDiv(expr, pos);
        skipSpaces(expr, pos);
        while (pos < expr.size() && (expr[pos] == '+' || expr[pos] == '-')) {
            char op = expr[pos++];
            double right = parseMulDiv(expr, pos);
            left = (op == '+') ? left + right : left - right;
            skipSpaces(expr, pos);
        }
        return left;
    }

    static double parseMulDiv(const string& expr, size_t& pos) {
        double left = parsePower(expr, pos);
        skipSpaces(expr, pos);
        while (pos < expr.size() && (expr[pos] == '*' || expr[pos] == '/')) {
            char op = expr[pos++];
            double right = parsePower(expr, pos);
            if (op == '/') {
                if (abs(right) < 1e-15) throw runtime_error("Division by zero");
                left /= right;
            }
            else left *= right;
            skipSpaces(expr, pos);
        }
        return left;
    }

    static double parsePower(const string& expr, size_t& pos) {
        double left = parseUnary(expr, pos);
        skipSpaces(expr, pos);
        if (pos < expr.size() && expr[pos] == '^') {
            pos++;
            double right = parsePower(expr, pos);
            left = pow(left, right);
        }
        return left;
    }

    static double parseUnary(const string& expr, size_t& pos) {
        skipSpaces(expr, pos);
        if (pos < expr.size() && expr[pos] == '-') { pos++; return -parseUnary(expr, pos); }
        if (pos < expr.size() && expr[pos] == '+') { pos++; return parseUnary(expr, pos); }
        return parseFunctionOrParen(expr, pos);
    }

    static double parseFunctionOrParen(const string& expr, size_t& pos) {
        skipSpaces(expr, pos);
        if (pos >= expr.size()) throw runtime_error("Unexpected end");
        if (tryFunc(expr, pos, "sin")) return sin(parseParen(expr, pos));
        if (tryFunc(expr, pos, "cos")) return cos(parseParen(expr, pos));
        if (tryFunc(expr, pos, "tan")) return tan(parseParen(expr, pos));
        if (tryFunc(expr, pos, "exp")) return exp(parseParen(expr, pos));
        if (tryFunc(expr, pos, "sqrt")) { double v = parseParen(expr, pos); if (v < 0) throw runtime_error("sqrt of negative"); return sqrt(v); }
        if (tryFunc(expr, pos, "ln")) { double v = parseParen(expr, pos); if (v <= 0) throw runtime_error("ln of non-positive"); return log(v); }
        if (tryFunc(expr, pos, "abs"))  return abs(parseParen(expr, pos));
        if (expr[pos] == '(') {
            pos++;
            double result = parseAddSub(expr, pos);
            skipSpaces(expr, pos);
            if (pos >= expr.size() || expr[pos] != ')') throw runtime_error("Missing )");
            pos++;
            return result;
        }
        return parseNumber(expr, pos);
    }

    static double parseParen(const string& expr, size_t& pos) {
        skipSpaces(expr, pos);
        if (pos >= expr.size() || expr[pos] != '(') throw runtime_error("Expected (");
        pos++;
        double result = parseAddSub(expr, pos);
        skipSpaces(expr, pos);
        if (pos >= expr.size() || expr[pos] != ')') throw runtime_error("Missing )");
        pos++;
        return result;
    }

    static bool tryFunc(const string& expr, size_t& pos, const string& name) {
        if (pos + name.size() <= expr.size() && expr.substr(pos, name.size()) == name) {
            size_t next = pos + name.size();
            if (next >= expr.size() || !isalpha(expr[next])) {
                pos = next;
                return true;
            }
        }
        return false;
    }

    static double parseNumber(const string& expr, size_t& pos) {
        skipSpaces(expr, pos);
        if (pos >= expr.size()) throw runtime_error("Unexpected end");
        size_t start = pos;
        bool hasDot = false;
        if (expr[pos] == '+' || expr[pos] == '-') pos++;
        while (pos < expr.size() && (isdigit(expr[pos]) || expr[pos] == '.')) {
            if (expr[pos] == '.') {
                if (hasDot) throw runtime_error("Invalid number");
                hasDot = true;
            }
            pos++;
        }
        if (pos == start || (pos == start + 1 && (expr[start] == '+' || expr[start] == '-')))
            throw runtime_error("Expected number at " + to_string(pos));
        return stod(expr.substr(start, pos - start));
    }

    static void skipSpaces(const string& expr, size_t& pos) {
        while (pos < expr.size() && isspace(expr[pos])) pos++;
    }
};