#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cctype>
#include <map>
#include <cstdlib>
#include "TAC.h"

using namespace std;

enum TokenType
{
    T_INT,       // 0
    T_ID,        // 1
    T_NUM,       // 2
    T_IF,        // 3
    T_ELSE,      // 4
    T_RETURN,    // 5
    T_ASSIGN,    // 6 ===> For =
    T_PLUS,      // 7
    T_MINUS,     // 8
    T_MUL,       // 9
    T_DIV,       // 10
    T_LPAREN,    // 11
    T_RPAREN,    // 12
    T_LBRACE,    // 13
    T_RBRACE,    // 14
    T_SEMICOLON, // 15
    T_GT,        // 16 ===> For >
    T_EOF,       // 17
    T_FOR,       // 18
    T_WHILE,     // 19
    T_EQ,        // 20  ===> For ==
    T_NEQ,       // 21 ===> For !=
    T_LE,        // 22  ===> For <=
    T_GE,        // 23  ===> For >=
    T_AND,       // 24 ===> For &&
    T_OR,        // 25  ===> For ||
    T_NOT,       // 26 ===> For !
    T_FLOAT,     // 27
    T_STRING,    // 28
    T_CHAR,      // 29
    T_UNDEFINED, // undefined
};

string getTokenName(TokenType type)
{
    switch (type)
    {
    case T_INT:
        return "int";
    case T_ID:
        return "identifier";
    case T_NUM:
        return "number";
    case T_IF:
        return "if";
    case T_ELSE:
        return "else";
    case T_RETURN:
        return "return";
    case T_ASSIGN:
        return "assignment op";
    case T_PLUS:
        return "plus";
    case T_MINUS:
        return "minus";
    case T_MUL:
        return "multiply";
    case T_DIV:
        return "divide";
    case T_LPAREN:
        return "(";
    case T_RPAREN:
        return ")";
    case T_LBRACE:
        return "{";
    case T_RBRACE:
        return "}";
    case T_SEMICOLON:
        return "semicolon";
    case T_GT:
        return "greater than";
    case T_EOF:
        return "end of file";
    case T_FOR:
        return "for";
    case T_WHILE:
        return "while";
    case T_EQ:
        return "equal";
    case T_NEQ:
        return "not equal";
    case T_LE:
        return "less than or equal";
    case T_GE:
        return "greater than or equal";
    case T_AND:
        return "and";
    case T_OR:
        return "or";
    case T_NOT:
        return "not";
    case T_FLOAT:
        return "float";
    case T_STRING:
        return "string";
    case T_CHAR:
        return "char";
    default:
        return "undefined";
    }
    return "";
}

struct Token
{
    TokenType type;
    string value;
    size_t lineNumber;

    // These attributes are only used to return values between functions
    string icgVariable;
};

class Lexer
{

private:
    string src;
    size_t position;
    size_t lineNumber;
    /*
    It hold positive values.
    In C++, size_t is an unsigned integer data type used to represent the
    size of objects in bytes or indices, especially when working with memory-related
    functions, arrays, and containers like vector or string. You can also use the int data type but size_t is recommended one
    */

public:
    Lexer(const string &src)
    {
        this->src = src;
        this->position = 0;
        this->lineNumber = 0;
    }

    vector<Token> tokenize()
    {
        vector<Token> tokens;
        while (position < src.size())
        {
            char current = src[position];

            if (current == '\n')
            {
                lineNumber++;
                position++;
                continue;
            }

            // Detect Single line comments
            if (current == '/' && src[position + 1] == '/')
            {
                while (src[position] != '\n' && position <= src.size())
                {
                    position++;
                }
                continue;
            }

            // Read string values
            if (current == '"')
            {
                position++;
                // string str = "\"";
                string str = "";
                while (src[position] != '"')
                {
                    str = str + src[position];
                    position++;
                }
                // str += src[position];
                position++;
                tokens.push_back(Token{T_STRING, str, lineNumber});
                continue;
            }

            if (isspace(current))
            {
                position++;
                continue;
            }
            if (isdigit(current))
            {
                tokens.push_back(Token{T_NUM, consumeNumber(), lineNumber});
                continue;
            }
            if (isalpha(current))
            {
                string word = consumeWord();
                if (word == "int")
                    tokens.push_back(Token{T_INT, word, lineNumber});
                else if (word == "float")
                    tokens.push_back(Token{T_FLOAT, word, lineNumber});
                else if (word == "string")
                    tokens.push_back(Token{T_STRING, word, lineNumber});
                else if (word == "if")
                    tokens.push_back(Token{T_IF, word, lineNumber});
                else if (word == "else")
                    tokens.push_back(Token{T_ELSE, word, lineNumber});
                else if (word == "return")
                    tokens.push_back(Token{T_RETURN, word, lineNumber});
                else if (word == "while")
                    tokens.push_back(Token{T_WHILE, word, lineNumber});
                else if (word == "for")
                    tokens.push_back(Token{T_FOR, word, lineNumber});
                else
                    tokens.push_back(Token{T_ID, word, lineNumber});
                continue;
            }

            // Add OPERATORS in the tokens vector
            switch (current)
            {
            case '=':
                tokens.push_back(Token{T_ASSIGN, "=", lineNumber});
                break;
            case '+':
                tokens.push_back(Token{T_PLUS, "+", lineNumber});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-", lineNumber});
                break;
            case '*':
                tokens.push_back(Token{T_MUL, "*", lineNumber});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/", lineNumber});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "(", lineNumber});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")", lineNumber});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{", lineNumber});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}", lineNumber});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", lineNumber});
                break;
            case '>':
                tokens.push_back(Token{T_GT, ">", lineNumber});
                break;
            // case '==':
            //     tokens.push_back(Token{T_EQ, "==", lineNumber});
            //     break;
            // case '>=':
            //     tokens.push_back(Token{T_GE, ">=", lineNumber});
            //     break;
            // case '<=':
            //     tokens.push_back(Token{T_GE, "<=", lineNumber});
            //     break;
            default:
                cout << "Unexpected character: " << current << endl;
                exit(1);
            }
            position++;
        }
        tokens.push_back(Token{T_EOF, "", lineNumber});
        return tokens;
    }

    string consumeNumber()
    {
        size_t start = position;
        while (position < src.size() && isdigit(src[position]))
            position++;
        return src.substr(start, position - start);
    }

    string consumeWord()
    {
        size_t start = position;
        while (position < src.size() && isalnum(src[position]))
            position++;
        return src.substr(start, position - start);
    }
};

class SymbolTable
{
public:
    void declareVariable(const string &name, const Token &symbolInstance)
    {
        if (symbolTable.find(name) != symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' is already declared.");
        }
        symbolTable[name] = symbolInstance;
    }

    void updateVariable(const string &name, const Token &symbolInstance)
    {
        if (symbolTable.find(name) == symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' not declared.");
        }
        symbolTable[name] = symbolInstance;
    }

    Token getVariableToken(const string &name)
    {
        if (symbolTable.find(name) == symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' is not declared.");
        }
        return symbolTable[name];
    }

    bool isDeclared(const string &name) const
    {
        return symbolTable.find(name) != symbolTable.end();
    }

    void displaySymbolTable()
    {
        cout << "\n    << -----------------Symbol Table----------------- >>" << endl;
        map<string, Token>::iterator it;
        for (it = symbolTable.begin(); it != symbolTable.end(); it++)
        {
            cout << " \t "
                 << "Type: " << getTokenName(it->second.type)
                 << " \t " << "Symbol: " << it->first
                 << " \t " << "Value: " << it->second.value << endl;
        }
        cout << "    << ---------------------------------------------- >>" << endl;
    }

private:
    map<string, Token> symbolTable;
};

class IntermediateCodeGenerator
{
public:
    vector<string> instructions;
    int tempCount = 0;
    int labelCount = 1;

    string newTemp()
    {
        return "t" + to_string(tempCount++);
    }

    string newLabel()
    {
        return "L" + to_string(labelCount++);
    }

    void addInstruction(const string &instr)
    {
        instructions.push_back(instr);
    }

    void printInstructions()
    {
        bool isBlockStarted = false;
        for (const auto &instr : instructions)
        {
            if (instr.find("L") != std::string::npos)
            {
                cout << instr << endl;
                isBlockStarted = true;
            }
            else if (instr.find("goto") != std::string::npos)
            {
                cout << instr << endl;
                isBlockStarted = false;
            }
            else
            {
                string output = isBlockStarted ? "    " + instr : instr;
                cout << output << endl;
            }
        }
    }
};

class Parser
{

public:
    Parser(const vector<Token> &tokens, SymbolTable &symbolTable, IntermediateCodeGenerator &icg)
        : tokens(tokens), position(0), symbolTable(symbolTable), icg(icg)
    {
        this->dataTypes[T_INT] = T_INT;
        this->dataTypes[T_STRING] = T_STRING;
        this->dataTypes[T_FLOAT] = T_FLOAT;
        this->dataTypes[T_CHAR] = T_CHAR;

        this->blockStatement[T_IF] = T_IF;
        this->blockStatement[T_WHILE] = T_WHILE;
        this->blockStatement[T_FOR] = T_FOR;
    }

    void parseProgram()
    {
        while (tokens[position].type != T_EOF)
        {
            // cout << "before: " << tokens[position].value << endl;
            parseStatement();
            // cout << "before: " << tokens[position].value << endl;
        }

        symbolTable.displaySymbolTable();
        cout << "\nParsing completed successfully! No Syntax Error\n"
             << endl;
    }

private:
    vector<Token> tokens;
    size_t position;
    map<TokenType, TokenType> dataTypes;
    map<TokenType, TokenType> blockStatement;
    SymbolTable &symbolTable;
    IntermediateCodeGenerator &icg;

    void parseStatement()
    {
        // cout << "tokens[position].value: " << tokens[position].value << endl;
        if (dataTypes.find(tokens[position].type) != dataTypes.end())
        {
            parseDeclaration(dataTypes[tokens[position].type]);
        }
        else if (tokens[position].type == T_ID)
        {
            parseAssignment();
        }
        else if (blockStatement.find(tokens[position].type) != blockStatement.end())
        {
            parseBlockStatement(tokens[position].type);
        }
        else if (tokens[position].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else if (tokens[position].type == T_LBRACE)
        {
            parseBlock();
        }
        else
        {
            cout << "Syntax error: unexpected token " << getQuotesAroundStr(tokens[position].value) << endl;
            exit(1);
        }
    }

    void parseBlock()
    {
        expect(T_LBRACE);
        while (tokens[position].type != T_RBRACE && tokens[position].type != T_EOF)
        {
            parseStatement();
        }
        expect(T_RBRACE);
    }

    void parseDeclaration(TokenType dataType)
    {
        expect(dataType);
        string identifierName = tokens[position].value;
        expect(T_ID);
        Token symbolInstance;
        if (tokens[position].type == T_ASSIGN)
        {
            expect(T_ASSIGN);
            if (tokens[position].type == T_STRING)
            {
                symbolInstance = Token{T_STRING, tokens[position].value};
                expect(T_STRING);
            }
            else if (tokens[position].type == T_NUM)
            {
                symbolInstance = parseAndEvaluateExpression();
            }
        }
        symbolInstance.type = dataType;
        symbolTable.declareVariable(identifierName, symbolInstance);
        expect(T_SEMICOLON);
        if (symbolInstance.value != "")
            icg.addInstruction(identifierName + " = " + symbolInstance.value);
    }

    string parseAssignment(bool generateIntermediateCode = true)
    {
        string symbol = tokens[position].value;
        expect(T_ID);
        Token symbolInstance = symbolTable.getVariableToken(symbol);
        if (tokens[position].type == T_PLUS || tokens[position].type == T_MINUS)
        {
            parseIncrementDecrementOperator(symbol, &symbolInstance);
        }
        else
        {
            expect(T_ASSIGN);
            symbolInstance = parseAndEvaluateExpression(symbolInstance);
            expect(T_SEMICOLON);
        }
        symbolTable.updateVariable(symbol, symbolInstance);
        string assignmentTo = symbolInstance.icgVariable == "" ? symbolInstance.value : symbolInstance.icgVariable;
        string icgInstruction = symbol + " = " + assignmentTo;
        if (generateIntermediateCode)
        {
            icg.addInstruction(icgInstruction); // Generate intermediate code for the assignment.
        }
        return icgInstruction;
    }

    void parseBlockStatement(TokenType blockStatementKeyword)
    {
        expect(blockStatementKeyword);
        expect(T_LPAREN);
        string loopStartLabel;
        if (blockStatementKeyword == T_FOR)
        {
            // Initialization / Declaration of iterator
            if (dataTypes.find(tokens[position].type) != dataTypes.end())
            {
                parseDeclaration(tokens[position].type);
            }
            else if (tokens[position].type == T_ID)
            {
                parseAssignment();
            }
        }

        if (blockStatementKeyword == T_WHILE || blockStatementKeyword == T_FOR)
        {
            // Starting label of FOR loop
            loopStartLabel = icg.newLabel();
            icg.addInstruction(loopStartLabel + ":");
        }

        // Evaluating condition
        Token condition = parseAndEvaluateExpression();

        string trueConditionLabel = icg.newLabel();
        string falseConditionLabel = icg.newLabel();
        icg.addInstruction("if " + condition.icgVariable + " goto " + trueConditionLabel);
        icg.addInstruction("goto " + falseConditionLabel);
        icg.addInstruction(trueConditionLabel + ":");

        string iteratorInstruction; // To receive instruction for iterator part of FOR loop
        if (blockStatementKeyword == T_FOR)
        {
            expect(T_SEMICOLON);
            iteratorInstruction = parseAssignment(false);
        }

        expect(T_RPAREN);

        parseStatement(); // Body of IF/FOR/WHILE

        if (blockStatementKeyword == T_FOR)
        {
            icg.addInstruction(iteratorInstruction); // Iterator instruction before going to start of loop
            icg.addInstruction("goto " + loopStartLabel);
            icg.addInstruction(falseConditionLabel + ":");
        }
        else if (blockStatementKeyword == T_WHILE)
        {
            icg.addInstruction("goto " + loopStartLabel);
            icg.addInstruction(falseConditionLabel + ":");
        }
        else if (blockStatementKeyword == T_IF && tokens[position].type == T_ELSE)
        {
            string elseLabel = icg.newLabel();
            icg.addInstruction("goto " + elseLabel);
            icg.addInstruction(falseConditionLabel + ":");
            expect(T_ELSE);
            parseStatement();
            icg.addInstruction(elseLabel + ":");
        }
    }

    void parseReturnStatement()
    {
        expect(T_RETURN);
        Token exp = parseAndEvaluateExpression();
        expect(T_SEMICOLON);
        string statement = exp.icgVariable == "" ? exp.value : exp.icgVariable;
        icg.addInstruction("return " + statement);
    }

    void parseIncrementDecrementOperator(string identifier, Token *identifierValue)
    {
        if (tokens[position].type == T_PLUS)
        {
            expect(T_PLUS);
            expect(T_PLUS);
            int value = stoi(identifierValue->value);
            identifierValue->value = to_string(value + 1);
            identifierValue->icgVariable = identifier + " + 1";
        }
        else if (tokens[position].type == T_MINUS)
        {
            expect(T_MINUS);
            expect(T_MINUS);
            int value = stoi(identifierValue->value);
            identifierValue->value = to_string(value - 1);
            identifierValue->icgVariable = identifier + " - 1";
        }
    }

    Token parseAndEvaluateExpression(Token initialValue = {})
    {
        Token result = initialValue;
        Token firstTerm = parseTerm();
        result.icgVariable = firstTerm.icgVariable == "" ? firstTerm.value : firstTerm.icgVariable;
        if (firstTerm.type == T_ID)
        {
            Token symbolInstance = symbolTable.getVariableToken(firstTerm.value);
            string identifierValue = symbolInstance.value;
            TokenType identifierType = symbolInstance.type;

            if (identifierValue == "")
                showErrorMessagesAndExit(getQuotesAroundStr(firstTerm.value) + " has value undefined!");

            result.value = identifierValue;
        }
        else
        {
            result.value = firstTerm.value;
        }
        while (tokens[position].type == T_PLUS || tokens[position].type == T_MINUS)
        {
            string op = tokens[position].type == T_PLUS ? "+" : "-";
            position++;
            Token nextTerm = parseTerm();
            string nextTermValue = nextTerm.value;
            TokenType nextTermType = result.type;
            if (nextTerm.type == T_ID)
            {
                Token tempSymbolInstance = symbolTable.getVariableToken(nextTerm.value);
                nextTermValue = tempSymbolInstance.value;
                nextTermType = tempSymbolInstance.type;
            }
            if (nextTermType != result.type) // Validation check for operation between different data types
                showErrorMessagesAndExit(
                    "Operation '" + op + "' cannot be applied between type: " + getTokenName(result.type) + " and " + getTokenName(nextTermType) + "!");
            if (op == "+")
            {
                if (result.type == T_INT)
                    result.value = to_string(stoi(result.value) + stoi(nextTermValue));
                else
                    result.value += nextTermValue;
            }
            else
            {
                if (result.type == T_INT)
                    result.value = to_string(stoi(result.value) - stoi(nextTermValue));

                else if (result.type == T_STRING)
                    showErrorMessagesAndExit("Cannot perform '-' op on type string");
            }
            string newVar = icg.newTemp();
            string nextTermVar = nextTerm.icgVariable == "" ? nextTerm.value : nextTerm.icgVariable;
            icg.addInstruction(newVar + " = " + result.icgVariable + " " + op + " " + nextTermVar);
            result.icgVariable = newVar;
        }
        if (tokens[position].type == T_GT)
        {
            position++;
            Token nextExp = parseAndEvaluateExpression();
            string nextExpVar = nextExp.icgVariable == "" ? nextExp.value : nextExp.icgVariable;
            string icgVar = icg.newTemp();
            icg.addInstruction(icgVar + " = " + result.icgVariable + " > " + nextExpVar);
            result.icgVariable = icgVar;
        }
        return result;
    }

    Token parseTerm()
    {
        Token factor = parseFactor();
        Token result = factor;
        if (factor.type == T_ID)
        {
            result = symbolTable.getVariableToken(factor.value);
            result.icgVariable = factor.value;
        }
        while (tokens[position].type == T_MUL || tokens[position].type == T_DIV)
        {
            // position++;
            // factor = parseFactor();

            TokenType op = tokens[position++].type;
            Token nextFactor = parseFactor();
            string nextFactorValue = nextFactor.value;
            if (nextFactor.type == T_ID)
            {
                Token symbolInstance = symbolTable.getVariableToken(nextFactor.value);

                if (symbolInstance.value == "")
                    showErrorMessagesAndExit(getQuotesAroundStr(nextFactor.value) + " has value undefined!");

                nextFactorValue = symbolInstance.value;
            }

            string resultStr = result.icgVariable == "" ? result.value : result.icgVariable;
            if (op == T_MUL)
            {
                result.value = to_string(stoi(result.value) * stoi(nextFactorValue));
            }
            else if (op == T_DIV)
            {
                result.value = to_string(stoi(result.value) / stoi(nextFactorValue));
            }

            string temp = icg.newTemp();
            icg.addInstruction(temp + " = " + resultStr + (op == T_MUL ? " * " : " / ") + nextFactor.value);
            result.icgVariable = temp;
        }
        return result;
    }

    Token parseFactor()
    {
        if (tokens[position].type == T_NUM || tokens[position].type == T_ID || tokens[position].type == T_STRING)
        {
            position++;
            return tokens[position - 1];
        }
        else if (tokens[position].type == T_LPAREN)
        {
            expect(T_LPAREN);
            Token exp = parseAndEvaluateExpression();
            expect(T_RPAREN);
            return exp;
        }
        else
        {
            cout << "Syntax error: unexpected token " << getQuotesAroundStr(tokens[position].value) << endl;
            exit(1);
        }
        return Token{};
    }

    void expect(TokenType type)
    {
        if (tokens[position].type == type)
        {
            position++;
        }
        else
        {
            showErrorMessagesAndExit(
                "Syntax error: expected " + getTokenName(type) + " but found " + tokens[position].value,
                "Error at line number: " + to_string(tokens[position].lineNumber));
            exit(1);
        }
    }

    template <typename... Args>
    void showErrorMessagesAndExit(const string &str, const Args &...args)
    {
        cout << "ERROR => " << str << endl;
        showErrorMessagesAndExit(args...);
    }

    void showErrorMessagesAndExit(const string &str)
    {
        cout << "ERROR => " << str << endl;
        exit(1);
    }

    string getQuotesAroundStr(string text)
    {
        return "'" + text + "'";
    }
};

int main()
{
    string input = R"(
        string a = "My name is";
        a = a + " Jawad";
        int sum;
        sum = 10 + 5 * 3;
        if(5 > 3){
            return 0;
        }
        else {
            return sum;
        }
        for (int i = 0; i > 10; i++) {
            string inLoop = "in loop";
        }
        int c = 0;
        while (c > 10) {
            int inner = 10;
        }
        int out = 0;
    )";

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    // for (size_t i = 0; i < tokens.size(); i++)
    // {
    //     cout << tokens[i].value << "\t" << getTokenName(tokens[i].type) << endl;
    // }

    SymbolTable symbolTable;
    IntermediateCodeGenerator icg;
    Parser parser(tokens, symbolTable, icg);

    parser.parseProgram();
    icg.printInstructions();

    return 0;
}