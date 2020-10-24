# Interpreter-1.0

**BRIEF DESCRIPTION:**

The project is derived in two vital parts:
1) Tokenizer- breaks the input into tokens,
which are saved in a queue

2) Interpreter - reads the tokens one by one 
and desides what to do with them.


**NOTES WHILE USING:**
1) It is required to put intervals after every 
Token, even after the last.(the Tokenizer design
requires it)


**STRUCTURES USED:**
1) Queue for the token list
   -pros:
     -easy to use and practical
2) linked list for the SymbolTable
   -pros:
     -easily accessible data, which is regularly
     getting modified
3) stack for the evaluation of the arithmetic 
expressions(reversed polish notation).

**MISSING ELEMENTS:**
1) WHILE,DONE,PUSH,POP
2) &&,||
3) Can't work in REPL


**Example of the syntax:**
```C++
LET count
READ count
IF count < 0
GOTO end
ENDIF

LET sum
sum = 0
WHILE count > 0
LET x
READ x
sum = sum + x
count = count - 1
DONE
PRINT sum

LABEL end
```
