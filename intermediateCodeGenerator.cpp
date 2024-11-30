#include <iostream>
#include <vector>

using namespace std;

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
