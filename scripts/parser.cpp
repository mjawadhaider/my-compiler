#include <vector>

using namespace std;

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
            else if (tokens[position].type == T_NUM || tokens[position].type == T_ID)
            {
                symbolInstance = parseAndEvaluateExpression();
            }
        }
        symbolInstance.type = dataType;
        symbolTable.declareVariable(identifierName, symbolInstance);
        expect(T_SEMICOLON);
        if (symbolInstance.icgVariable != "")
            icg.addInstruction(identifierName + " = " + symbolInstance.icgVariable);
        else if (symbolInstance.value != "")
        {
            if (symbolInstance.type == T_STRING)
            {
                symbolInstance.value = "\"" + symbolInstance.value + "\"";
            }
            icg.addInstruction(identifierName + " = " + symbolInstance.value);
        }
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
            bool isNextTermIdentifier = false;
            Token nextTerm = parseTerm(&isNextTermIdentifier);
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
            cout << "isNextTermIdentifier: " << isNextTermIdentifier << endl;
            if (result.type == T_STRING && !isNextTermIdentifier)
            {
                nextTermVar = "\"" + nextTermVar + "\"";
            }
            icg.addInstruction(newVar + " = " + result.icgVariable + " " + op + " " + nextTermVar);
            result.icgVariable = newVar;
        }
        // if (tokens[position].type == T_GT || tokens[position].type == T_LT || tokens[position].type == T_EQ)
        if (isComparisonOperator(tokens[position].type))
        {
            TokenType comparisonOp = tokens[position].type;
            position++;
            Token nextExp = parseAndEvaluateExpression();
            string nextExpVar = nextExp.icgVariable == "" ? nextExp.value : nextExp.icgVariable;
            string icgVar = icg.newTemp();
            icg.addInstruction(icgVar + " = " + result.icgVariable + " " + getTokenName(comparisonOp) + " " + nextExpVar);
            result.icgVariable = icgVar;
        }
        return result;
    }

    Token parseTerm(bool *isNextTermIdentifier = nullptr)
    {
        Token factor = parseFactor();
        Token result = factor;
        if (factor.type == T_ID)
        {
            result = symbolTable.getVariableToken(factor.value);
            result.icgVariable = factor.value;
            if (isNextTermIdentifier != nullptr)
                *isNextTermIdentifier = true;
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
                if (isNextTermIdentifier != nullptr)
                    *isNextTermIdentifier = true;
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