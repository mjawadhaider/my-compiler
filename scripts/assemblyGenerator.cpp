#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>

using namespace std;

class AssemblyGenerator {
public:
    vector<string> assemblyCode; // Holds the generated assembly instructions
    map<string, string> tempToRegister; // Map for temp variables to registers
    int registerCount = 0; // For tracking used registers

    string getRegister() {
        return "R" + to_string(registerCount++);
    }

    void generateAssembly(const vector<string> &tacInstructions) {
        for (const string &instr : tacInstructions) {
            if (instr.find(" = ") != string::npos) {
                handleAssignment(instr);
            } else if (instr.find("if ") != string::npos) {
                handleConditionalJump(instr);
            } else if (instr.find("goto") != string::npos) {
                handleUnconditionalJump(instr);
            } else if (instr.find("return") != string::npos) {
                handleReturn(instr);
            } else if (instr.find(":") != string::npos) {
                handleLabel(instr);
            }
        }
    }

    void handleAssignment(const string &instr) {
        // Example: t0 = x + y
        size_t eqPos = instr.find("=");
        string lhs = trim(instr.substr(0, eqPos));
        string rhs = trim(instr.substr(eqPos + 1));

        if (rhs.find("+") != string::npos || rhs.find("-") != string::npos ||
            rhs.find("*") != string::npos || rhs.find("/") != string::npos) {
            // Handle arithmetic
            handleArithmetic(lhs, rhs);
        } else {
            // Simple assignment
            string reg = getRegister();
            assemblyCode.push_back("MOV " + reg + ", " + rhs);
            tempToRegister[lhs] = reg;
        }
    }

    void handleArithmetic(const string &lhs, const string &rhs) {
        // Example: t0 = x + y
        string reg1 = getRegister();
        size_t opPos;
        string op;

        if ((opPos = rhs.find("+")) != string::npos) op = "ADD";
        else if ((opPos = rhs.find("-")) != string::npos) op = "SUB";
        else if ((opPos = rhs.find("*")) != string::npos) op = "MUL";
        else if ((opPos = rhs.find("/")) != string::npos) op = "DIV";

        string operand1 = trim(rhs.substr(0, opPos));
        string operand2 = trim(rhs.substr(opPos + 1));

        assemblyCode.push_back("MOV " + reg1 + ", " + operand1);
        assemblyCode.push_back(op + " " + reg1 + ", " + operand2);
        tempToRegister[lhs] = reg1;
    }

    void handleConditionalJump(const string &instr) {
        // Example: if t0 goto L1
        size_t ifPos = instr.find("if");
        size_t gotoPos = instr.find("goto");
        string condition = trim(instr.substr(ifPos + 2, gotoPos - ifPos - 2));
        string label = trim(instr.substr(gotoPos + 4));

        assemblyCode.push_back("CMP " + condition + ", 1");
        assemblyCode.push_back("JNE " + label);
    }

    void handleUnconditionalJump(const string &instr) {
        // Example: goto L1
        size_t gotoPos = instr.find("goto");
        string label = trim(instr.substr(gotoPos + 4));
        assemblyCode.push_back("JMP " + label);
    }

    void handleReturn(const string &instr) {
        // Example: return x
        size_t retPos = instr.find("return");
        string value = trim(instr.substr(retPos + 6));
        assemblyCode.push_back("MOV R0, " + value);
        assemblyCode.push_back("RET");
    }

    void handleLabel(const string &instr) {
        // Example: L1:
        assemblyCode.push_back(instr);
    }

    string trim(const string &s) {
        size_t start = s.find_first_not_of(" \t");
        size_t end = s.find_last_not_of(" \t");
        return (start == string::npos) ? "" : s.substr(start, end - start + 1);
    }

    void writeToFile(const string &outputFile) {
        ofstream out(outputFile);
        for (const auto &line : assemblyCode) {
            out << line << endl;
        }
        out.close();
        cout << "Assembly Code written to " << outputFile << endl;
    }

    void printAssembly() {
        for (const auto &line : assemblyCode) {
            cout << line << endl;
        }
    }
};
