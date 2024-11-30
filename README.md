# MyCompiler

`MyCompiler` is a custom compiler built as part of a **Compiler Construction** course project. It is capable of lexically analyzing, parsing, and generating intermediate code in the form of **Three-Address Code (TAC)** for a subset of programming constructs. The compiler supports control structures such as `if`, `else`, `for`, and `while` loops, and can handle arithmetic expressions and variable assignments.

---

## **Features**
1. **Lexer**:
   - Tokenizes the input source code into meaningful components such as keywords, identifiers, operators, and literals.
   - Detects and ignores comments and whitespace.
   - Reports errors for unexpected characters.

2. **Parser**:
   - Validates the syntax of the tokenized input.
   - Handles declarations, assignments, and control structures.
   - Builds an abstract syntax tree (implicitly) for subsequent code generation.

3. **Symbol Table**:
   - Tracks declared variables, their types, and their values.
   - Prevents redeclaration and ensures variables are initialized before use.
   - Displays all declared variables for debugging purposes.

4. **Intermediate Code Generator (ICG)**:
   - Generates **Three-Address Code (TAC)** for valid input programs.
   - Supports:
     - Arithmetic operations.
     - Relational operations.
     - Control flow (e.g., `if`, `for`, `while`).
   - Reuses temporary variables and labels for optimization.

---

## **Modules**
### **1. Lexer**
- Located in the `Lexer` class.
- Tokenizes the input into:
  - **Keywords**: `int`, `string`, `if`, `while`, etc.
  - **Identifiers**: Variable names.
  - **Operators**: `+`, `-`, `*`, `/`, `=`, etc.
  - **Literals**: Numeric and string constants.
- Outputs a list of tokens for the parser.

### **2. Parser**
- Located in the `Parser` class.
- Validates the syntax of the tokenized input based on language grammar.
- Handles:
  - Declarations (`int x;`)
  - Assignments (`x = 10;`)
  - Conditional Statements (`if`, `else`)
  - Loops (`for`, `while`)
  - Return statements (`return x;`)

### **3. Symbol Table**
- Located in the `SymbolTable` class.
- Tracks:
  - Variable names, types, and values.
  - Variable declarations and updates.
- Reports semantic errors like:
  - Use of undeclared variables.
  - Redeclaration of variables.

### **4. Intermediate Code Generator**
- Located in the `IntermediateCodeGenerator` class.
- Generates **Three-Address Code (TAC)**:
  - Simplifies expressions into linear instructions.
  - Manages control flow with labels and conditional jumps.
- Optimizes by reusing temporary variables and labels.

---

## **How to Run**

1. **Compile the Compiler:**:
   `g++ main.cpp -o main`

2. **Run the Compiler:**:
   `./main -o output.txt example.jwd`
