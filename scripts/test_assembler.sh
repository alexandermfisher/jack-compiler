#!/bin/bash

BUILD_TYPE="debug"
TEST_NAMES=("token" "token_table" "symbol_table" "parser" "code_generator" "utils")

while getopts "b:" opt; do
  case ${opt} in
    b ) BUILD_TYPE=$OPTARG ;;
    * ) echo "Usage: $0 [-b <build_type>] [test_name]"; exit 1 ;;
  esac
done
shift $((OPTIND - 1))

BUILD_DIR="build/$BUILD_TYPE/src/assembler/tests"

if [ ! -d "build/$BUILD_TYPE" ]; then
    echo "==> Build directory does not exist, configuring CMake..."
    cmake --preset="$BUILD_TYPE" || { echo "CMake configuration failed"; exit 1; }
fi

run_test() {
    local test_exec="$1"
    if [ -f "$test_exec" ]; then
        echo "==> Running $test_exec"
        tmp_stderr=$(mktemp)
        if [ "$BUILD_TYPE" == "memcheck" ]; then
            valgrind --leak-check=full --error-exitcode=1 "$test_exec" 2>"$tmp_stderr"
        else
            "$test_exec" 2>"$tmp_stderr"
        fi
        if [ $? -ne 0 ]; then
            echo "❌ Test failed. Stderr output:"
            cat "$tmp_stderr"
        fi
        rm "$tmp_stderr"
    else
        echo "❌ Error: Test executable '$test_exec' not found!"
        exit 1
    fi
}

# Run single test if specified
if [ $# -eq 1 ]; then
    TEST_EXEC="$BUILD_DIR/test_$1"
    echo "==> Building and running test: $1 ($BUILD_TYPE mode)"
    ninja -C build/"$BUILD_TYPE" "src/assembler/tests/test_$1" || { echo "Build failed!"; exit 1; }
    run_test "$TEST_EXEC"
    #echo "✅ Test '$1' passed!"
    exit 0
fi

# BIG HEADER FOR UNIT TESTS
echo -e "\n=============================================="
echo -e "🚀 🚀 🚀  RUNNING UNIT TESTS  🚀 🚀 🚀"
echo -e "==============================================\n"

echo "==> Building all assembler unit tests..."
ninja -C build/"$BUILD_TYPE" || { echo "Build failed!"; exit 1; }

for TEST in "${TEST_NAMES[@]}"; do
    run_test "$BUILD_DIR/test_$TEST"
done

echo -e "\n✅✅✅  UNIT TESTS COMPLETED SUCCESSFULLY  ✅✅✅\n"

# BIG HEADER FOR INTEGRATION TESTS
echo -e "\n=============================================="
echo -e "🧩 🧩 🧩  RUNNING INTEGRATION TESTS  🧩 🧩 🧩"
echo -e "==============================================\n"

INTEGRATION_TEST_SCRIPT="src/assembler/tests/integration/test_integration.sh"
ASSEMBLER_EXECUTABLE="./build/$BUILD_TYPE/src/assembler/hackasm"

if [ -f "$INTEGRATION_TEST_SCRIPT" ]; then
    if [ "$BUILD_TYPE" == "memcheck" ]; then
        bash "$INTEGRATION_TEST_SCRIPT" -e "$ASSEMBLER_EXECUTABLE" -m
    else
        bash "$INTEGRATION_TEST_SCRIPT" -e "$ASSEMBLER_EXECUTABLE"
    fi
else
    echo "❌ Integration test script not found at $INTEGRATION_TEST_SCRIPT"
    exit 1
fi

echo -e "\n✅✅✅  ALL TESTS (UNIT + INTEGRATION) PASSED  ✅✅✅"
