#include "expression.h"
#include "stack.h"

#include <cctype>
#include <string>


int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return -1;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool isOperandCharacter(unsigned char c) {
    return std::isalnum(c) != 0 || c == '_';
}


std::string convertInfixToPostfix(const std::string& expression) {
    StackData operators;
    std::string output;
    bool expectingOperand = true;
    bool foundOperand = false;

    const auto fail = [&operators]() {
        clearStack(operators);
        return std::string("ERROR");
    };

    for (std::size_t i = 0; i < expression.size();) {
        const unsigned char raw = static_cast<unsigned char>(expression[i]);
        const char current = expression[i];
        if (std::isspace(raw)) {
            ++i;
            continue;
        }

        if (isOperandCharacter(raw)) {
            if (!expectingOperand) return fail();
            const std::size_t start = i;
            while (i < expression.size() &&
                   isOperandCharacter(static_cast<unsigned char>(expression[i]))) {
                ++i;
            }
            output.append(expression, start, i - start);
            output += ' ';
            expectingOperand = false;
            foundOperand = true;
            continue;
        }

        if (current == '(') {
            if (!expectingOperand) return fail();
            stackPush(operators, "(");
            ++i;
            continue;
        }

        if (current == ')') {
            if (expectingOperand) return fail();
            bool openingFound = false;
            while (const auto top = stackPeek(operators)) {
                if (*top == "(") {
                    stackPop(operators);
                    openingFound = true;
                    break;
                }
                output += *stackPop(operators);
                output += ' ';
            }
            if (!openingFound) return fail();
            ++i;
            expectingOperand = false;
            continue;
        }

        if (!isOperator(current) || expectingOperand) return fail();
        while (const auto top = stackPeek(operators)) {
            if (*top == "(" || priority((*top)[0]) < priority(current)) break;
            output += *stackPop(operators);
            output += ' ';
        }
        stackPush(operators, std::string(1, current));
        ++i;
        expectingOperand = true;
    }

    if (!foundOperand || expectingOperand) return fail();
    while (const auto top = stackPop(operators)) {
        if (*top == "(") return fail();
        output += *top;
        output += ' ';
    }
    if (!output.empty() && output.back() == ' ') output.pop_back();
    clearStack(operators);
    return output;
}
