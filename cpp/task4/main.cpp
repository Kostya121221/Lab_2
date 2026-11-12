#include "wildcard.h"
#include <iostream>
#include <string>
int main(){std::string text,pattern;std::getline(std::cin,text);std::getline(std::cin,pattern);std::cout<<(wildcardMatch(text,pattern)?"MATCH":"NO MATCH")<<'\n';}
