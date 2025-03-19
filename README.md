# 📚 **Project Overview: jack-compiler**

This repository is designed for the **Nand2Tetris Hack platform** and focuses on building the core components of a **Jack-to-Hack compilation pipeline**. The project is **modular**, supporting unit testing, memory checking, and clean CLI utility design.

### 🧠 **Planned Compiler Stages**
The ultimate goal is to build a full compilation pipeline from Jack source code to Hack machine code. The following components are planned:

| Stage                | Status                 | Description                                           |
|----------------------|------------------------|-------------------------------------------------------|
| **Hack Assembler**   | ✅ **Completed**        | Translates Hack Assembly (`.asm`) → Hack Machine Code (`.hack`) |
| **VM Translator**    | 🚧 **To Do**            | Translates VM code (`.vm`) → Hack Assembly (`.asm`)   |
| **Jack Compiler**    | 🚧 **To Do**            | Compiles Jack source (`.jack`) → VM code (`.vm`)      |

---

## ✅ **Requirements**

### 📦 **Build System**
The project uses **CMake ≥ 3.20** and assumes a **POSIX-compliant system** with:

✅ **POSIX Compliance Requirements:**
- Support for **command-line tools** like `find`, `diff`, `sh`, and `make`
- **Bash shell** or compatible for running build/test scripts
- Optional: **valgrind** or **asan** for memory checks

✅ **Compiler and Tools:**
- **GCC** or **Clang** supporting **C11**
- **CMake** ≥ 3.20
- **Ninja**
- **Valgrind** (optional, for memory leak checking)
- **AddressSanitizer (ASan)** support (enabled automatically in Debug mode)

The build system enforces **C11 standard**:
```cmake
cmake_minimum_required(VERSION 3.20)
project(jack-compiler C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
```

---

### 🧠 **Build Types & Flags**
The build system supports multiple modes:

| **Build Type** | **Description**                                                                                      |
|--------------- |------------------------------------------------------------------------------------------------------|
| **Release**    | Optimized build (`-O2 -march=native -flto`) **[default]**                                             |
| **Debug**      | Debug symbols, stack protection, no optimizations (`-g -O0 -DDEBUG`)                                  |
| **MemCheck**   | Valgrind-friendly build (`-fno-omit-frame-pointer -fstack-protector-strong`)                         |

✅ If `ENABLE_MEMCHECK=ON`, memory-check-friendly flags are enabled.  
✅ **ASan** is automatically enabled in **Debug mode** unless **MemCheck** is explicitly selected.

---

### 🔧 **Designing the CLI Utility**

#### 1. **Separate Binaries (Current)**
Each stage is built as a standalone executable:
- ✅ `hackasm`: Hack assembler
- 🚧 `vmtrans`: VM Translator (WIP)
- 🚧 `jackc`: Jack Compiler (WIP)

Usage Example:
```bash
./jackc MyProgram.jack           # (future)
./vmtrans MyProgram.vm           # (future)
./hackasm MyProgram.asm -o MyProgram.hack
```

#### 2. **Unified Compiler (Future Goal)**
Eventually, a unified `jackcc` CLI will:
- **Auto-detect input types**
- **Chain the compilation steps**

Example:
```bash
./jackcc MyProgram.jack -o MyProgram.hack
```

---

## ⚙️ **Assembler (`hackasm`)**

The **Hack Assembler** translates `.asm` files into `.hack` machine code.

### 🏗 **Building the Assembler**
Builds are managed by `scripts/build.sh`:
```bash
./scripts/build.sh [-b <build_type>] [-c]
```

| Flag                  | Description                                                                 |
|-----------------------|-----------------------------------------------------------------------------|
| `-b <build_type>`     | Specify build type (`debug`, `release`, `memcheck`). Default: `release`.     |
| `-c`                  | Clean the `bin/` directory and exit.                                        |

Example Usage:
```bash
./scripts/build.sh                  # Build release (default)
./scripts/build.sh -b debug         # Build debug mode
./scripts/build.sh -b memcheck      # Build for Valgrind
./scripts/build.sh -c               # Clean bin/
```

✅ Output binary: `bin/hackasm`

---

## 🧪 **Running Tests**
Unit and integration tests are run via:
```bash
./scripts/test_assembler.sh [-b <build_type>] [test_name]
```

| Option                 | Description                                                                                   |
|------------------------|-----------------------------------------------------------------------------------------------|
| `-b <build_type>`      | Build type (`debug`, `release`, or `memcheck`). Default: `debug`.                              |
| `test_name`            | (Optional) Run a specific test (`token`, `parser`, `code_generator`, etc.)                     |

Example Usage:
```bash
./scripts/test_assembler.sh                          # Run all tests (debug mode)
./scripts/test_assembler.sh -b memcheck              # Run tests with Valgrind
./scripts/test_assembler.sh token                    # Run only the 'token' test
```

✅ **Valgrind** runs automatically with `-b memcheck`

---

✅ **Integration Testing**  
Integration tests live at:
```
src/assembler/tests/integration/test_integration.sh
```
It runs `.asm` samples and compares output `.hack` files for correctness.

---

## 🖥️ **VM Translator (`vmtrans`)**

The **VM Translator** converts **VM commands** into **Hack Assembly**.

### 🚧 **Current Development Status**
> **Status:** 🛠 In Progress

Planned features:
- Parse `.vm` files
- Translate **stack operations**, **memory access**, **function calls**
- Handle **single files** and **directories of .vm files**

Expected CLI:
```bash
./vmtrans MyProgram.vm -o MyProgram.asm
./vmtrans MyFolder/ -o MyProgram.asm
```

---

## 🧠 **Planned Jack Compiler (`jackc`)**

Future plans for a **recursive descent Jack compiler**:
- Lexical analysis (tokenizer)
- Syntax parsing (recursive descent parser)
- Code generation (to VM)
- Symbol table management
- Output `.vm` files

---


