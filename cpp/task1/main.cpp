#include <iostream>
#include <string>
#include "expression.h"
int main() { std::string expression; std::getline(std::cin, expression); std::cout << convertInfixToPostfix(expression) << '\n'; }
