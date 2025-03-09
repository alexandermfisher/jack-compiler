#!/bin/bash

# Default build type
BUILD_TYPE="debug"

# List of test names to run (easily editable)
TEST_NAMES=("assembler" "parser" "token" "token_table" "code" "symbol_table" "utils" "line_processor")  # Add test names here

# Parse options
while getopts "b:" opt; do
  case ${opt} in
    b ) BUILD_TYPE=$OPTARG ;;
    * ) echo "Usage: $0 [-b <build_type>] [test_name]"; exit 1 ;;
  esac
done
shift $((OPTIND - 1))  # Remove processed options

# Set build directory (update to reflect correct path)
BUILD_DIR="build/$BUILD_TYPE/src/assembler/tests"

# Ensure build directory exists
if [ ! -d "build/$BUILD_TYPE" ]; then
    echo "==> Build directory does not exist, configuring CMake..."
    cmake --preset="$BUILD_TYPE" || { echo "CMake configuration failed"; exit 1; }
fi

# Function to run tests (with or without Valgrind)
run_test() {
    local test_exec="$1"

    if [ -f "$test_exec" ]; then
        echo "==> Running $test_exec"

        # Create a temporary file to capture stderr
        tmp_stderr=$(mktemp)

        # Run the test executable and capture stderr
        if [ "$BUILD_TYPE" == "memcheck" ]; then
            valgrind --leak-check=full --error-exitcode=1 "$test_exec" 2>"$tmp_stderr"
        else
            "$test_exec" 2>"$tmp_stderr"
        fi

        # Check if the test executable failed (non-zero exit code)
        if [ $? -ne 0 ]; then
            echo "Test failed. Capturing stderr output:"
            cat "$tmp_stderr"  # Display captured stderr
        fi

        # Clean up the temporary stderr file
        rm "$tmp_stderr"
    else
        echo "Error: Test executable '$test_exec' not found!"
        exit 1
    fi
}

# If a test name is provided, only build and run that test
if [ $# -eq 1 ]; then
    TEST_EXEC="$BUILD_DIR/test_$1"
    echo "==> Building and running test: $1 ($BUILD_TYPE mode)"
    ninja -C build/"$BUILD_TYPE" "src/assembler/tests/test_$1" || { echo "Build failed!"; exit 1; }
    run_test "$TEST_EXEC"
    exit 0
fi

# If no test is specified, build all and run them
echo "==> No test specified, building and running all assembler tests..."
ninja -C build/"$BUILD_TYPE" || { echo "Build failed!"; exit 1; }

# Run all tests in the TEST_NAMES list
for TEST in "${TEST_NAMES[@]}"; do
    run_test "$BUILD_DIR/test_$TEST"
done
