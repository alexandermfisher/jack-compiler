#!/bin/bash

# Paths
ASSEMBLER_EXECUTABLE="../../cmake-build-debug/src/assembler/hackasm"
TEST_DIR="$(dirname "$0")/test_programs"

# Default mode: run all tests
ONLY_NO_SYMBOLS=false

if [[ "$1" == "--no-symbols" ]]; then
    ONLY_NO_SYMBOLS=true
fi

# Track test results
PASSED=0
FAILED=0

# Iterate over all test cases
for TEST_CASE in "$TEST_DIR"/*; do
    if [ -d "$TEST_CASE" ]; then
        BASE_NAME=$(basename "$TEST_CASE")
        OUTPUT_HACK="$TEST_CASE/output.hack"

        # Test `L.asm` (no symbols)
        L_ASM_FILE="$TEST_CASE/${BASE_NAME}L.asm"
        EXPECTED_L_HACK="$TEST_CASE/${BASE_NAME}-test.hack"

        if [[ -f "$L_ASM_FILE" ]]; then
            echo "🛠 Running test: $BASE_NAME (No Symbols: $(basename "$L_ASM_FILE"))"
            "$ASSEMBLER_EXECUTABLE" "$L_ASM_FILE" "$OUTPUT_HACK"

            if diff -q "$EXPECTED_L_HACK" "$OUTPUT_HACK"; then
                echo "✅ PASSED: $BASE_NAME (No Symbols)"
                ((PASSED++))
            else
                echo "❌ FAILED: $BASE_NAME (No Symbols)"
                diff "$EXPECTED_L_HACK" "$OUTPUT_HACK"
                ((FAILED++))
            fi
        fi

        # If `--no-symbols` is set, skip testing `.asm`
        if [[ "$ONLY_NO_SYMBOLS" == true ]]; then
            continue
        fi

        # Test `.asm` (with symbols)
        ASM_FILE="$TEST_CASE/${BASE_NAME}.asm"
        EXPECTED_HACK="$TEST_CASE/${BASE_NAME}-test.hack"

        if [[ -f "$ASM_FILE" ]]; then
            echo "🛠 Running test: $BASE_NAME (With Symbols: $(basename "$ASM_FILE"))"
            "$ASSEMBLER_EXECUTABLE" "$ASM_FILE" "$OUTPUT_HACK"

            if diff -q "$EXPECTED_HACK" "$OUTPUT_HACK"; then
                echo "✅ PASSED: $BASE_NAME (With Symbols)"
                ((PASSED++))
            else
                echo "❌ FAILED: $BASE_NAME (With Symbols)"
                diff "$EXPECTED_HACK" "$OUTPUT_HACK"
                ((FAILED++))
            fi
        fi
    fi
done

# Summary
echo "------------------------------------"
echo "🏆 Test Summary: $PASSED Passed, $FAILED Failed"
echo "------------------------------------"

# Exit with failure if any tests failed
exit $FAILED