#!/bin/bash

# Colors for output
GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[1;33m"
RESET="\033[0m"

# Paths
ASSEMBLER_EXECUTABLE="${ASSEMBLER_EXECUTABLE:-../../../cmake-build-debug/src/assembler/hackasm}"
TEST_DIR="$(dirname "$0")/test_programs"
OUTPUT_DIR="$(dirname "$0")/output"

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Modes
ONLY_NO_SYMBOLS=false
USE_VALGRIND=false

# Parse arguments
while [[ "$#" -gt 0 ]]; do
    case "$1" in
        --no-symbols) ONLY_NO_SYMBOLS=true ;;
        --valgrind) USE_VALGRIND=true ;;
        *) echo -e "${RED}Unknown argument: $1${RESET}"; exit 1 ;;
    esac
    shift
done

# Track test results
PASSED=0
FAILED=0

# Iterate over test cases
for TEST_CASE in "$TEST_DIR"/*; do
    if [[ -d "$TEST_CASE" ]]; then
        BASE_NAME=$(basename "$TEST_CASE")
        OUTPUT_HACK="$OUTPUT_DIR/${BASE_NAME}.hack"

        # Run tests with or without symbols
        for MODE in "L" ""; do
            if [[ "$MODE" == "L" ]]; then
                ASM_FILE="$TEST_CASE/${BASE_NAME}L.asm"
                EXPECTED_HACK="$TEST_CASE/${BASE_NAME}-test.hack"
                MODE_DESC="(No Symbols)"
            else
                ASM_FILE="$TEST_CASE/${BASE_NAME}.asm"
                EXPECTED_HACK="$TEST_CASE/${BASE_NAME}-test.hack"
                MODE_DESC="(With Symbols)"
                [[ "$ONLY_NO_SYMBOLS" == true ]] && continue
            fi

            # Skip if the file doesn't exist
            if [[ ! -f "$ASM_FILE" ]]; then
                continue
            fi

            echo -e "${YELLOW}🛠 Running test: $BASE_NAME $MODE_DESC${RESET}"

            # Run the assembler
            if [[ "$USE_VALGRIND" == true ]]; then
                valgrind --leak-check=full --error-exitcode=1 "$ASSEMBLER_EXECUTABLE" "$ASM_FILE" "$OUTPUT_HACK"
            else
                "$ASSEMBLER_EXECUTABLE" "$ASM_FILE" "$OUTPUT_HACK"
            fi

            # Compare output
            if diff "$EXPECTED_HACK" "$OUTPUT_HACK"; then
                echo -e "${GREEN}✅ PASSED: $BASE_NAME $MODE_DESC${RESET}"
                ((PASSED++))
            else
                echo -e "${RED}❌ FAILED: $BASE_NAME $MODE_DESC${RESET}"
                ((FAILED++))
            fi
        done
    fi
done

# Summary
echo "------------------------------------"
echo -e "🏆 ${GREEN}Test Summary: $PASSED Passed${RESET}, ${RED}$FAILED Failed${RESET}"
echo "------------------------------------"

# Exit with failure if any tests failed
exit $FAILED