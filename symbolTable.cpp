#include <map>
#include <string>

using namespace std;

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
