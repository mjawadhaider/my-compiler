#include <string>

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