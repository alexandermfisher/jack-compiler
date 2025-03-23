Designing the CLI Utility

A fully-fledged compiler would typically wrap everything into a single tool that can go from Jack source code to Hack machine code. However, since you’re implementing each stage separately, you could take one of two approaches:
1.	Separate Binaries: Each stage (jackc, vmtrans, hackasm) is its own executable. Users manually run them in sequence.
2.	Unified Compiler: A single CLI tool (jackcc) that detects input type and runs the required passes internally.

make clean api interface for assembler, innit it with config and file paths and token file path, and log file path

assembler_init -> validates input files access, and sets config 
assembler_assmeble -> initiates assembly
assmebler_write_log(file stream or sdout or sdeer)
defualt log file is tmp dir overiden by argument log path in init and can then  log on errors by caller/driver code 




<command> ::= <arithmetic> | <memory> | <program_flow> | <function_call>
<arithmetic> ::= "add" | "sub" | "neg" | "eq" | "gt" | "lt" | "and" | "or" | "not"
<memory> ::= ("push" | "pop") <segment> <index>
<segment> ::= "argument" | "local" | "static" | "constant" | "this" | "that" | "pointer" | "temp"
<program_flow> ::= "label" <label> | "goto" <label> | "if-goto" <label>
<function_call> ::= "function" <function_name> <nVars> | "call" <function_name> <nArgs> | "return"
<index> ::= integer (0 or positive)
<function_name> ::= (alphanumeric with . or _ allowed)
<label> ::= (alphanumeric with . or _ allowed)





# 1️⃣ Configure the project (choose preset: debug, release, memcheck)
cmake --preset=debug
cmake --preset=release
cmake --preset=memcheck

# 2️⃣ Build the project
cmake --build --preset=debug
cmake --build --preset=release
cmake --build --preset=memcheck

# 3️⃣ Run the executable
./build/debug/your_executable_name
./build/release/your_executable_name
./build/memcheck/your_executable_name

# 4️⃣ Run tests (if applicable)
ctest --preset=debug
ctest --preset=release
ctest --preset=memcheck

# 5️⃣ Clean the build
cmake --build --preset=debug --target clean
rm -rf build/  # Manual cleanup

# 6️⃣ Debugging (gdb for Linux, lldb for macOS)
gdb ./build/debug/your_executable_name   # Linux
lldb ./build/debug/your_executable_name  # macOS