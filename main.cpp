#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "utils.cpp"
#include "lexer.cpp"
#include "symbolTable.cpp"
#include "intermediateCodeGenerator.cpp"
#include "parser.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cerr << "Usage: " << argv[0] << " -o <output-file> <input-file>" << endl;
        return 1;
    }

    string inputFileName = argv[3];
    string outputFileName = argv[2];

    ifstream inputFile(inputFileName);
    if (!inputFile.is_open())
    {
        cerr << "Error: Could not open file " << inputFileName << endl;
        return 1;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string input = buffer.str();
    inputFile.close();

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
    // icg.printInstructions();

    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not write to file " << outputFileName << endl;
        return 1;
    }

    // Output the intermediate code
    for (const auto &instr : icg.instructions) {
        outputFile << instr << endl;
    }

    outputFile.close();
    cout << "Compilation completed successfully. Intermediate code written to " << outputFileName << endl;
    
    return 0;
}