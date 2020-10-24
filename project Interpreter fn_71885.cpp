// project Interpreter fn_71885.cpp 
#include <iostream>
#include<fstream>
#include <string>
#include<cassert>
#include "Queue.hpp"
#include "Stack.hpp"
#include "Tokenizer.hpp"
#include "Interpreter.hpp"


int main()
{
	Tokenizer tz("input.txt");
	Interpreter prs(tz.getAllTokens());
	prs.visit();
	return 0;
}

