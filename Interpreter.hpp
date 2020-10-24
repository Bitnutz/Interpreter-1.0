#pragma once
///********************PART2***************************
///Interpreter
///******BOOLEANS FUNCTIONS VALIDATING SOME THINGS
bool isArithmeticOperator(TokenType t)
{
	return t == Plus || t == Minus || t == Multiplicate || t == Divide || t == DecimalDivide;
}

bool isArithmeticExpression(TokenType t)
{
	return t == Plus || t == Minus || t == Multiplicate || t == Divide || t == DecimalDivide || t == Number;
}
bool isComparisonOperator(TokenType t)
{
	return t == Equal || t == NotEqual || t == Greater || t == GreaterOrEqual || t == LessThan || t == LessOrEqual;
}
///***************************************************


/// helper function for evaluateArithmetiExpression()
/// converts string to int
int stringToInt(std::string str)
{
	int result = 0;
	for (int i = 0; str[i] != '\0'; i++)
	{
		result = result * 10 + str[i] - '0';
	}
	return result;
}
///***********************LINKED LIST**************************************
struct listNode
{
	Token id;
	int valueInt;
	int scopeLevel;
	listNode* next;

	listNode(Token id) :id(id), valueInt(0), scopeLevel(0), next(nullptr) {}
	//these setters are used when we declare a variable and when we encounter IF/ELSE statement
	void setValue(int newVal)
	{
		valueInt = newVal;
	}
	void setScopeLevel(int level)
	{
		scopeLevel = level;
	}
};

class linkedList
{
private:
	listNode* front;
	listNode* back;
	int size;

	void del()
	{
		while (front)
		{
			popFront();
		}
	}

public:
	linkedList() :front(nullptr), back(nullptr), size(0) {}
	~linkedList()
	{
		del();
	}

	bool isEmpty()const
	{
		return size == 0;
	}
	//returns true if the Token is found in the table
	bool isDeclared(Token& t)
	{
		listNode* temp = front;
		while (temp != nullptr)
		{
			if (temp->id.value == t.value)
				return true;
			temp = temp->next;
		}
		return false;
	}
	void pushBack(const Token& element)
	{
		if (isEmpty())
		{
			listNode* temp = new listNode(element);
			front = temp;
			back = temp;
		}
		else
		{
			listNode* temp = new listNode(element);
			back->next = temp;
			back = temp;
		}
		size++;
	}
	void pushFront(const Token& element)
	{
		if (isEmpty())
		{
			listNode* temp = new listNode(element);
			front = temp;
			back = temp;
		}
		else
		{
			listNode* temp = new listNode(element);
			temp->next = front;
			front = temp;
		}
		size++;
	}

	void popFront()
	{
		if (isEmpty())
		{
			std::cerr << "Can't pop an empty list\nFigure out what's the error!\n";
			return;
		}
		else
		{
			listNode* temp = front;
			front = front->next;
			delete temp;
			size--;
		}
	}
	//removes the variables from the given scopeLevel
	void removeByScopeLevel(int level)
	{
		if (size == 1 && front->scopeLevel == level)
		{
			popFront();
			return;
		}
		while (front->scopeLevel == level)
			popFront();
	}

	void modifyValue(Token& t, int newVal)
	{
		listNode* temp = front;

		while (temp != nullptr)
		{
			if (temp->id.value == t.value)
			{
				temp->setValue(newVal);
				break;
			}
			temp = temp->next;
		}
	}
	//this function is used right after we add a variable in the table
	void setScopeLevelToTheLastAdded(int level)
	{
		front->setScopeLevel(level);
	}
	int getValueOf(Token& t)
	{
		listNode* temp = front;
		while (temp != nullptr)
		{
			if (temp->id.value == t.value)
				return temp->valueInt;
			temp = temp->next;
		}
		return -1;//can't happen
	}
};
///***********************LINKED LIST**************************************
///************************** END *****************************************

class Interpreter
{
private:
	Queue<Token> tokenList;
	linkedList symbolTable;
	Token curToken;
	stack<bool> comparisonExprs; //here we store the comparison expressions, we pop them to see if we have to execute the ELSE statement
	int curScopeLevel; // keeps track of the current scopeLevel in the program
	bool foundError;  //if error occurs while parsing and interpreting, set this flag to true
public:
	Interpreter(Queue<Token>tl) :tokenList(tl), symbolTable(), comparisonExprs()
	{
		curToken = tokenList.dequeue();
		curScopeLevel = 0;
		foundError = 0;
	}
	//essential function for this class
	void getNextToken()
	{
		if (!tokenList.isEmpty())
			curToken = tokenList.dequeue();
		else
		{
			curToken.type = InvalidToken;
		}
	}

	///Evaluates an arithmetic expression using the reversed polish notation
	int evaluateArithmeticExpression()
	{
		stack<int> stNumber;
		stack<std::string> stOperators;
		int lhs = 0;
		int rhs = 0;

		if (curToken.type == Not)
		{
			getNextToken();
			if (curToken.type == Number)
				lhs = !stringToInt(curToken.value);
			if (curToken.type == Identifier)
				lhs = !symbolTable.getValueOf(curToken);
			return lhs;
		}
		if (curToken.type == InvalidToken)
		{
			foundError = 1;
			std::cerr << "Invalid Token found.\nMight be due to missed white space or new line\n.";
			return 0;
		}

		while (isArithmeticExpression(curToken.type) || curToken.type == Identifier)
		{
			if (curToken.type == Number)
			{
				stNumber.push(stringToInt(curToken.value));
				getNextToken();
				//if the next token is number or identifier we break and evaluate the pushed numbers so far
				//this is for situations such as:
				// LET a = 10
				// a = a + 5
				if (curToken.type == Number || curToken.type == Identifier)
					break;
			}
			else if (curToken.type == Identifier && symbolTable.isDeclared(curToken))
			{
				stNumber.push(symbolTable.getValueOf(curToken));
				getNextToken();
				if (curToken.type == Number || curToken.type == Identifier)
					break;
			}
			else
			{
				stOperators.push(curToken.value);
				getNextToken();
				if (isArithmeticOperator(curToken.type))
				{
					std::cerr << "Invalid use of arithmetic operators\n";
					foundError = 1;
					return -1;
				}
			}
		}
		if (stNumber.getSize() - 1 != stOperators.getSize()) //check if the arithmetic expression is all right
		{
			foundError = 1;
			std::cerr << "Invalid use of arithmetic operators\n";
			return -1;
		}
		while (!stOperators.isEmpty())
		{
			if (stOperators.peek() == "+")
			{
				rhs = stNumber.pop();
				lhs = stNumber.pop();
				stOperators.pop();
				lhs += rhs;
				stNumber.push(lhs);
				if (stOperators.isEmpty())
					break;
			}
			if (stOperators.peek() == "-")
			{
				rhs = stNumber.pop();
				lhs = stNumber.pop();
				stOperators.pop();
				lhs -= rhs;
				stNumber.push(lhs);
				if (stOperators.isEmpty())
					break;
			}
			if (stOperators.peek() == "*")
			{
				rhs = stNumber.pop();
				lhs = stNumber.pop();
				stOperators.pop();
				lhs *= rhs;
				stNumber.push(lhs);
				if (stOperators.isEmpty())
					break;
			}
			if (stOperators.peek() == "/")
			{
				rhs = stNumber.pop();
				lhs = stNumber.pop();
				stOperators.pop();
				if (rhs == 0)
				{
					std::cerr << "Can't divide a number by zero\n";
					foundError = 1;
					return -1;
				}
				lhs /= rhs;
				stNumber.push(lhs);


				if (stOperators.isEmpty())
					break;
			}
			if (stOperators.peek() == "%")
			{
				rhs = stNumber.pop();
				lhs = stNumber.pop();
				stOperators.pop();
				lhs %= rhs;
				stNumber.push(lhs);
				if (stOperators.isEmpty())
					break;
			}
		}
		if (stNumber.getSize() == 1)
			return stNumber.peek();
		else
		{
			foundError = 1;
			std::cerr << "Invalid use of arithmetic operators\n";
			return 0;
		}
	}

	bool evaluateComparisonExpression()
	{
		//evaluateAritmeticExpression() and save it in lhs
		//getNextToken()
		//curToken.type is comparison operator
		//getNextToken() and if it's not a number, error
		//evaluateArithmeticExpression() and compare it with temp, according to the given comparison operator

		int lhs = 0; //left hand side
		int rhs = 0; //right hand side

		if (curToken.type == Identifier)
			lhs = evaluateArithmeticExpression();
		if (curToken.type == Number)
			lhs = evaluateArithmeticExpression();
		if (curToken.type == Not)
			lhs = evaluateArithmeticExpression();



		if (isComparisonOperator(curToken.type) && curToken.type != Not)
		{
			TokenType compOperatorType = curToken.type;
			getNextToken();

			if (curToken.type == Number || curToken.type == Identifier)
				rhs = evaluateArithmeticExpression();

			if (compOperatorType == Equal)
				return lhs == rhs;
			if (compOperatorType == NotEqual)
				return lhs != rhs;
			if (compOperatorType == Greater)
				return lhs > rhs;
			if (compOperatorType == GreaterOrEqual)
				return lhs >= rhs;
			if (compOperatorType == LessThan)
				return lhs < rhs;
			if (compOperatorType == Equal)
				return lhs <= rhs;
		}
		else // the statement is like: IF !a
		{
			return lhs || rhs;
		}
		return false;
	}


	///goes through all tokens and desides what to do with them
	void visit()
	{
		while (!tokenList.isEmpty() && !foundError)
		{
			//################ INVALID TOKEN #####################
			if (curToken.type == InvalidToken)
			{
				foundError = 1;
				std::cerr << "Invalid Token found.\nMight be due to:\n -missed white space or new line.\n -Invalid declaration of a variable\n -the use of Invalid Token\n";
				break;
			}
			//################ LET #####################
			if (curToken.type == LET)
			{
				getNextToken();
				if (curToken.type == Identifier)
				{
					if (symbolTable.isDeclared(curToken))
					{
						std::cerr << "'" << curToken.value << "' is already declared.\nDeclaring a variable more than once is forbidden\n";
						foundError = 1;
						break;
					}
					else
					{
						symbolTable.pushFront(curToken);
						symbolTable.setScopeLevelToTheLastAdded(curScopeLevel);
					}
				}
			}
			//################ READ #####################
			if (curToken.type == READ)
			{
				getNextToken();
				if (curToken.type == Identifier && symbolTable.isDeclared(curToken)) //if its identifier and this identifier is declared
				{
					int num;
					//std::cin >> num;
					if (!(std::cin >> num))
					{
						std::cerr << "Can't input anything different from number.\n";
						foundError = 1;
						break;
					}
					symbolTable.modifyValue(curToken, num);
				}
				else
				{
					std::cerr << "error: Invalid syntax\nExpected Identifier after READ\n";
					foundError = 1;
					break;
				}
			}
			//################ PRINT #####################
			if (curToken.type == PRINT)
			{
				getNextToken();
				if (curToken.type == Identifier && symbolTable.isDeclared(curToken))
				{
					std::cout << symbolTable.getValueOf(curToken) << std::endl;
				}
				else
				{
					std::cerr << "Can't print token '" << curToken.value << "' because it's not declared\n";
					foundError = 1;
					break;
				}
			}
			//################ IDENTIFIER #####################
			if (curToken.type == Identifier)
			{
				if (!symbolTable.isDeclared(curToken))
				{
					std::cerr << "Can't use token '" << curToken.value << "' because it's not declared\n";
					foundError = 1;
					break;
				}
				Token temp = curToken;//save the identifier to modify it if there's an expression

				getNextToken();
				if (curToken.type == Assign)
				{
					getNextToken();
					if (!(curToken.type == Number || curToken.type == Identifier))
					{
						std::cerr << "There has to be a rvalue after an assignment operator\n";
						foundError = 1;
						break;
					}
					else
						symbolTable.modifyValue(temp, evaluateArithmeticExpression());

				}
			}
			if (curToken.type == Assign)
			{
				std::cerr << "Invalid syntax error\n";
				foundError = 1;
				return;
			}
			//################ IF #####################
			if (curToken.type == IF)
			{
				getNextToken();
				if (!(curToken.type == Number || curToken.type == Identifier || curToken.type == Not))
				{
					std::cerr << "Invalid syntax,\nExpected a comparison expression after keyword IF\n";
					foundError = 1;
					break;
				}
				//save tmp in the comparison stack
				bool tmp = evaluateComparisonExpression();
				comparisonExprs.push(tmp);
				//increment the scope level
				curScopeLevel++;

				if (curToken.type == InvalidToken)
				{
					foundError = 1;
					std::cerr << "Invalid Token found.\nMight be due to:\n -missed white space or new line.\n -Invalid declaration of a variable\n -the use of Invalid Token\n";
					break;
				}
				//if the compExpression was false don't execute the code in the IF statement
				if (!tmp)
				{
					while (curToken.type != ELSE && curToken.type != ENDIF)
					{
						getNextToken();
						if (curToken.type == EOI)
						{
							std::cerr << "Every IF statement have to end with an ENDIF\n";
							foundError = 1;
							break;
						}
					}
				}
			}
			//################ ELSE #####################
			if (curToken.type == ELSE)
			{
				if (comparisonExprs.peek() == 1) //if the last comparison expression was true, don't execute the code in the ELSE block
				{
					while (curToken.type != ENDIF)
					{
						getNextToken();
						if (curToken.type == EOI)
						{
							std::cerr << "Every IF statement have to end with an ENDIF\n";
							foundError = 1;
							break;
						}
					}
				}
				else
				{
					symbolTable.removeByScopeLevel(curScopeLevel);
					getNextToken();
				}
			}
			//################ ENDIF #####################
			if (curToken.type == ENDIF)
			{
				comparisonExprs.pop(); //get rid of the last comparison expr in the stack
				symbolTable.removeByScopeLevel(curScopeLevel); //here we clean the variables in the local scope
				curScopeLevel--;
				getNextToken();
			}
			//################ WHILE #####################
			if (curToken.type == WHILE)
			{
				std::cout << "WHILE is not implemented\n";
				foundError = 1;
				return;
			}
			//################ DONE #####################
			if (curToken.type == DONE)
			{
				std::cout << "DONE is not implemented\n";
				foundError = 1;
				return;
			}
			//################ GOTO #####################
			if (curToken.type == GOTO)
			{
				while (curToken.type != LABEL)
					getNextToken();
				if (curToken.type == EOI)
				{
					std::cerr << "Can't use GOTO without LABEL\n";
					foundError = 1;
					break;
				}
			}
			//################ LABEL #####################
			if (curToken.type == LABEL)
			{
				getNextToken();
			}
		}
	}
};