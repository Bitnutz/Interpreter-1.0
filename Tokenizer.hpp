#pragma once
/*
@INFO:
	- IDENTIFIER - it's like in c++
	- NUMBER - an integer
	- OPERATOR - >,>=,<,<=,==,!=,!,=,+,-,*,/,%,||,&&
*/

/*
@CURRENT PROBLEMS:
	- fix getNextToken

	- finish the lexical analysis
*/

///***************************BOOLEAN FUNCTIONS CHECKING IF A STRING OR A SYMBOL IS SOMETHING***********************************
//**
bool isKeyword(std::string& str)
{
	return str == "LABEL" || str == "GOTO" || str == "LET" || str == "READ" || str == "PRINT" || str == "IF" || str == "ENDIF"
		|| str == "ELSE" || str == "WHILE" || str == "DONE" || str == "PUSH" || str == "POP";
}
bool isLetter(char c)
{
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}
bool isDigit(char c)
{
	return c >= '0' && c <= '9';
}
bool isOperator(char c)
{
	return c == '==' || c == '!=' || c == '>=' || c == '<=' || c == '>' || c == '<' || c == '+' || c == '-' || c == '*' || c == '/'
		|| c == '=' || c == '!' || c == '|' || c == '&';
}
bool isComparisonOperator(char c)
{
	return c == '=' || c == '>' || c == '<' || c == '!';
}
bool isArithmeticOperator(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}
bool isWhiteSpace(char c)
{
	return c == ' ';
}
bool isNewLine(char c)
{
	return c == '\n';
}
//**
///******************************************************************************************

///**************************THE DIFFERENT TYPES OF TOKEN****************************************
enum TokenType
{
	Identifier, Number, EOI, //EOI == end of input
	InvalidToken,

	//Boolelan operators
	And, Or, Not, //(&&, ||,!)

	 //Comparison operators
	 GreaterOrEqual, Greater, LessOrEqual, LessThan, Equal, NotEqual,
	 //Assign
	 Assign,
	 //Arithmetic Operators
	 Plus, Minus, Multiplicate, Divide, DecimalDivide,

	 //Keywords
	 Keyword,
	 LABEL, GOTO, LET, READ, PRINT, IF, ENDIF, ELSE, WHILE, DONE, PUSH, POP
};

struct Token
{
	TokenType type; //the type can be: Keyword,Number,Operator
	std::string value; //the actual keyword or operator or number as a string

	Token(TokenType t, std::string v) :type(t), value(v) {}
	Token() {}
};


///*************
///		The Tokenizer is meant to break down the input into lexemes and therefore into a different tokens
///************************************************************************************************************************************
///IMPORTANT:
///		THIS LANGUAGE VALUES THE USAGE OF SPACES
///	VALID INPUT: LET a = 3 + 3
///	INVALID INPUT: LET a=3+3
///**************************************************************************************************************************************
class Tokenizer
{
private:
	std::ifstream input; //the source
	int fileLength; //the length of the file 
	int position; // current position of the input pointer(tellg())

	//******Helper functions for getNextToken();
	Token recognizeKeyword() //might recognize an identifier as well
	{
		std::string toStore;

		while (!isWhiteSpace(input.peek()) && !isNewLine(input.peek()) && input.peek() != '\0')
		{
			toStore += input.peek();
			position++;
			input.ignore();
		}
		return recognizeIdentifier();
	}

	Token recognizeIdentifier()
	{
		std::string toStore;
		while (position < fileLength)
		{
			char c = input.peek();
			if (!(isLetter(c) || isDigit(c) || c == '_'))
				break;
			toStore += c;
			position++;
			input.ignore();
		}

		if (isKeyword(toStore))
		{
			if (toStore == "LABEL")
				return Token(LABEL, toStore);
			if (toStore == "GOTO")
				return Token(GOTO, toStore);
			if (toStore == "LET")
				return Token(LET, toStore);
			if (toStore == "READ")
				return Token(READ, toStore);
			if (toStore == "PRINT")
				return Token(PRINT, toStore);
			if (toStore == "IF")
				return Token(IF, toStore);
			if (toStore == "ELSE")
				return Token(ELSE, toStore);
			if (toStore == "ENDIF")
				return Token(ENDIF, toStore);
			if (toStore == "WHILE")
				return Token(WHILE, toStore);
			if (toStore == "PUSH")
				return Token(PUSH, toStore);
			if (toStore == "POP")
				return Token(POP, toStore);
		}
		return Token(Identifier, toStore);
	}

	Token recognizeNumber()
	{
		char c = input.peek();
		std::string toStore;

		if (c == '0') //if the number is for example 098, we return invalid token
		{
			toStore += c;
			input.ignore();
			if (isDigit(input.peek()))
				return Token(InvalidToken, "");
		}
		while (isDigit(input.peek()))
		{
			toStore += input.peek();
			position++;
			input.ignore();
		}
		if (input.peek() != ' ' && input.peek() != '\n') //if the input contains something like: 98f, we return invalid token
		{
			char d = input.peek();
			input >> toStore;
			return Token(InvalidToken, "");
		}
		return Token(Number, toStore);
	}

	Token recognizeComparisonOperator()
	{
		int pos = position;
		char c = input.peek();
		char nextChar;
		std::string toStore; // store the operator then pass it to the token
		toStore += c;
		if (pos + 1 > fileLength)
			nextChar = '\0';
		else
		{
			input.ignore();
			nextChar = input.peek();
		}

		if (nextChar == '=')
		{
			toStore += nextChar;
			input.ignore();
		}

		position += toStore.length();

		if (input.peek() != ' ') //spaces are essential
		{
			skipIntervalsAndNewLines();
			input >> toStore;
			return Token(InvalidToken, "");
		}

		if (toStore == ">=")
		{
			return Token(GreaterOrEqual, toStore);
		}
		if (toStore == "<=")
		{
			return Token(LessOrEqual, toStore);
		}
		if (toStore == ">")
		{
			return Token(Greater, toStore);
		}
		if (toStore == "<")
		{
			return Token(LessThan, toStore);
		}
		if (toStore == "==")
		{
			return Token(Equal, toStore);
		}
		if (toStore == "!=")
		{
			return Token(NotEqual, toStore);
		}
		if (toStore == "=")
		{
			return Token(Assign, toStore);
		}
		if (toStore == "!")
		{
			return Token(Not, toStore);
		}
		else
		{
			input >> toStore;
			return Token(InvalidToken, "");
		}
	}

	Token recognizeArithmeticOperator()
	{
		char c = input.peek();
		std::string toStore;
		toStore += c;

		position++; //here the operators can't be two-signed so we increment position just with one
		input.ignore();
		if (input.peek() != ' ') //if there is no space after the operator we return invalid token
		{
			skipIntervalsAndNewLines();
			input >> toStore;
			input.ignore(toStore.length(), input.peek() == ' ' || input.peek() == '\n');
			return Token(InvalidToken, "");
		}
		if (toStore == "+")
		{
			return Token(Plus, toStore);
		}
		if (toStore == "-")
		{
			return Token(Minus, toStore);
		}
		if (toStore == "*")
		{
			return Token(Multiplicate, toStore);
		}
		if (toStore == "/")
		{
			return Token(Divide, toStore);
		}
		if (toStore == "%")
		{
			return Token(DecimalDivide, toStore);
		}
	}

	Token recognizeOperator()
	{
		char c = input.peek();

		if (isComparisonOperator(c)) //here we count include '=' and '!' as well
			return recognizeComparisonOperator();
		if (isArithmeticOperator(c))
			return recognizeArithmeticOperator();

	}

	void skipIntervalsAndNewLines()
	{
		int pos = position;
		char c = input.peek();
		while (isWhiteSpace(c) || isNewLine(c))
		{
			if (isWhiteSpace(c))
			{
				position++;
				input.ignore();
			}
			if (isNewLine(c))
			{
				position += 2; //because newLine = '\n' (2 symbols)
				input.ignore();
			}
			c = input.peek();
		}
	}
	//*******
public:
	Tokenizer(std::string fileName)
	{
		input.open(fileName, std::ios::in);
		assert(input);
		//to know the length of the file
		input.seekg(0, std::ios::end);
		fileLength = input.tellg();
		input.seekg(0);

		position = 0;
	}

	Token getNextToken()
	{
		if (isWhiteSpace(input.peek()) || isNewLine(input.peek()))
			skipIntervalsAndNewLines();
		if (position >= fileLength || input.eof())
			return Token(EOI, "end of input\n");

		char c = input.peek();

		if (isLetter(c))
			return recognizeIdentifier();
		if (isDigit(c))
			return recognizeNumber();
		if (isOperator(c))
			return recognizeOperator();
	}
	///Function checking if all the tokens are correct
	/*bool isLexicallyCorrect()
	{
		Token* t = &getNextToken();
		while (t->type != EOI)
		{
			if (t->type == InvalidToken)
				return false;
			t = &getNextToken();
		}
		return true;
	}*/

	Queue<Token> getAllTokens()
	{
		Queue<Token> st;
		Token t = getNextToken();
		while (t.type != EOI)
		{
			st.enqueue(t);
			t = getNextToken();
		}
		st.enqueue(t); //here we are putting the EOI token as well
		return st;
	}



	~Tokenizer()
	{
		input.close();
		fileLength = 0;
		position = 0;
	}
};