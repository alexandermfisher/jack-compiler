Designing the CLI Utility

A fully-fledged compiler would typically wrap everything into a single tool that can go from Jack source code to Hack machine code. However, since you’re implementing each stage separately, you could take one of two approaches:
1.	Separate Binaries: Each stage (jackc, vmtrans, hackasm) is its own executable. Users manually run them in sequence.
2.	Unified Compiler: A single CLI tool (jackcc) that detects input type and runs the required passes internally.


TODO:
- main:
  function briefs
  test header functions 
  integration tests
  readme
- maybe generalise token table using opaque token struct










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