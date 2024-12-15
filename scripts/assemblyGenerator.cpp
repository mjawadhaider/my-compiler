#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>

using namespace std;

class AssemblyGenerator
{
private:
    vector<string> assemblyCode;            // Holds the generated assembly code
    map<string, string> variableToRegister; // Maps variables to registers
    map<string, string> registerToVariable; // Maps registers to variables
    vector<string> availableRegisters;      // Pool of available x86 registers
    set<string> tempVariables;              // Tracks temporary variables

public:
    AssemblyGenerator()
    {
        // Initialize available x86 registers
        availableRegisters = {"EAX", "EBX", "ECX", "EDX"};
    }

    // Generate x86 assembly code from TAC
    // Generate x86 assembly code from TAC
    void generateAssembly(const vector<string> &tacLines, const string &outputFile)
    {
        for (const string &line : tacLines)
        {
            string trimmedLine = trim(line);
            vector<string> tokens = split(trimmedLine, ' ');

            // cout << "------------------ Start-----------------------" << endl;
            // for (size_t i = 0; i < tokens.size(); i++)
            // {
            //     cout << "tokens[i]: " << tokens[i] << endl;
            // }
            // cout << "------------------ End-----------------------" << endl;
            

            if (tokens.empty()) continue; // Skip empty lines

            // Handle different TAC instructions
            if (tokens.size() == 3 && tokens[1] == "=") {
                handleAssignment(tokens);
            }
            else if (tokens.size() == 5 && (tokens[3] == "+" || tokens[3] == "-" || tokens[3] == "*" || tokens[3] == "/")) {
                handleArithmetic(tokens);
            }
            else if (tokens.size() == 4 && tokens[0] == "if") {
                handleConditionalJump(tokens);
            }
            else if (tokens.size() == 2 && tokens[0] == "goto") {
                handleUnconditionalJump(tokens);
            }
            else if (tokens.size() == 1 && tokens[0].back() == ':') {
                handleLabel(tokens);
            }
            else if (tokens.size() == 2 && tokens[0] == "return") {
                handleReturn(tokens);
            }
            else if (tokens.size() == 5 && (tokens[3] == ">" || tokens[3] == "<")) {
                handleComparison(tokens);
            }
            else {
                cerr << "Error: Unrecognized TAC instruction: " << line << endl;
            }
        }
        // Write all assembly instructions to the file
        writeToFile(outputFile);
    }

    // Handle simple assignments: a = b
    void handleAssignment(const vector<string> &tokens)
    {
        string dest = tokens[0];
        string src = tokens[2];
        assemblyCode.push_back("    MOV " + getRegister(dest) + ", " + src);
    }

    // Handle arithmetic operations: temp = a + b, a - b, etc.
    void handleArithmetic(const vector<string> &tokens)
    {
        string dest = tokens[0];
        string left = tokens[2];
        string right = tokens[4];
        string op = tokens[3];

        string leftReg = getRegister(left);

        // Load left operand into the register
        assemblyCode.push_back("    MOV " + leftReg + ", " + left);

        if (op == "+") {
            assemblyCode.push_back("    ADD " + leftReg + ", " + right);
        } else if (op == "-") {
            assemblyCode.push_back("    SUB " + leftReg + ", " + right);
        } else if (op == "*") {
            assemblyCode.push_back("    IMUL " + leftReg + ", " + right);
        } else if (op == "/") {
            assemblyCode.push_back("    IDIV " + right);
        }

        // Store the result back to the destination
        assemblyCode.push_back("    MOV " + dest + ", " + leftReg);
    }

    // Handle conditional jumps: if temp goto L1
    void handleConditionalJump(const vector<string> &tokens)
    {
        string condition = tokens[1];
        string label = tokens[3];
        assemblyCode.push_back("    CMP " + condition + ", 0");
        assemblyCode.push_back("    JNE " + label);
    }

    // Handle unconditional jumps: goto L1
    void handleUnconditionalJump(const vector<string> &tokens)
    {
        string label = tokens[1];
        assemblyCode.push_back("    JMP " + label);
    }

    // Handle labels: L1:
    void handleLabel(const vector<string> &tokens)
    {
        assemblyCode.push_back(tokens[0]);
    }

    // Handle return statements: return value
    void handleReturn(const vector<string> &tokens)
    {
        string value = tokens[1];
        assemblyCode.push_back("    MOV eax, " + value);
        assemblyCode.push_back("    int 0x80"); // Exit syscall
    }

    // Handle comparisons: temp = a > b or temp = a < b
    void handleComparison(const vector<string> &tokens)
    {
        string dest = tokens[0];
        string left = tokens[2];
        string right = tokens[4];
        string op = tokens[3];

        string leftReg = getRegister(left);

        assemblyCode.push_back("    MOV " + leftReg + ", " + left);
        assemblyCode.push_back("    CMP " + leftReg + ", " + right);

        if (op == ">") {
            assemblyCode.push_back("    SETg AL");
        } else if (op == "<") {
            assemblyCode.push_back("    SETl AL");
        }

        assemblyCode.push_back("    MOVzx " + dest + ", AL");
    }

    // Helper function to get a register for a variable
    string getRegister(const string &var)
    {
        if (variableToRegister.find(var) == variableToRegister.end()) {
            string reg = availableRegisters.back();
            availableRegisters.pop_back();
            variableToRegister[var] = reg;
        }
        return variableToRegister[var];
    }

    // Write the generated assembly code to a file
    void writeToFile(const string &outputFile)
    {
        ofstream asmFile(outputFile);
        for (const auto &line : assemblyCode) {
            asmFile << line << endl;
        }
        asmFile.close();
        cout << "Assembly code generated in " << outputFile << endl;
    }

    // Helper function: Split a string by a delimiter
    vector<string> split(const string &line, char delimiter)
    {
        vector<string> tokens;
        string token;
        istringstream tokenStream(line);
        cout << "line.find('\"'): " << line.find('\"') << endl;
        if (line.find('\"') != string::npos) {
            
        }
        while (getline(tokenStream, token, delimiter)) {
            cout << "token: " << token << endl;
            if (!token.empty()) tokens.push_back(token);
        }
        return tokens;
    }

    // Helper function: Trim whitespace from a string
    string trim(const string &str)
    {
        size_t start = str.find_first_not_of(" \t");
        size_t end = str.find_last_not_of(" \t");
        return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
    }

    void printAssembly()
    {
        for (const auto &line : assemblyCode)
        {
            cout << line << endl;
        }
    }
};
