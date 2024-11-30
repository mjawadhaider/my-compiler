#include <iostream>
#include <string>
#include <vector>

using namespace std;

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