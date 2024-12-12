#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "scripts/utils.cpp"
#include "scripts/lexer.cpp"
#include "scripts/symbolTable.cpp"
#include "scripts/intermediateCodeGenerator.cpp"
#include "scripts/parser.cpp"
#include "scripts/assemblyGenerator.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " -o <input-file>" << endl;
        return 1;
    }

    string inputFileName = argv[2];

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

    // Lexical Analysis
    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    // Symbol Table, ICG, and Parser
    SymbolTable symbolTable;
    IntermediateCodeGenerator icg;
    Parser parser(tokens, symbolTable, icg);

    parser.parseProgram();
    cout << "\nCompilation completed successfully." << endl;

    icg.writeToOutputFile("output/TAC-Output.txt");

    // Generate Assembly
    AssemblyGenerator asmGen;
    asmGen.generateAssembly(icg.instructions);
    asmGen.writeToFile("output/Assembly-Output.txt");
    
    return 0;
}