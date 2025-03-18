#!/bin/bash

set -o pipefail

ASSEMBLER_EXEC=""
MEMCHECK=false
TEST_DIR="$(dirname "$0")/test_programs"
TMP_DIR=$(mktemp -d)

usage() {
  echo "Usage: $0 -e <assembler_exec> [-m (enable valgrind memcheck)]"
  exit 1
}

while getopts "e:m" opt; do
  case ${opt} in
    e ) ASSEMBLER_EXEC=$OPTARG ;;
    m ) MEMCHECK=true ;;
    * ) usage ;;
  esac
done

if [[ -z "$ASSEMBLER_EXEC" ]]; then
  usage
fi

echo "==> Running integration tests"
echo "Assembler: $ASSEMBLER_EXEC"
echo "Test programs directory: $TEST_DIR"
echo "Valgrind Memcheck: $MEMCHECK"

for test_case in "$TEST_DIR"/*; do
  if [[ -d "$test_case" ]]; then
    test_name=$(basename "$test_case")
    echo ""
    echo "------------------------------------------"
    echo "Testing case: $test_name"
    echo "------------------------------------------"

    for asm_file in "$test_case"/*.asm; do
      asm_base=$(basename "$asm_file" .asm)
      asm_output="$TMP_DIR/${asm_base}.hack"

      # Expected output is always per test case (single expected file)
      expected_output="$test_case/${test_name}-test.hack"

      echo "--> Compiling: $asm_file"

      if [[ "$MEMCHECK" = true ]]; then
              valgrind_log=$(mktemp)
              valgrind --leak-check=full --error-exitcode=99 "$ASSEMBLER_EXEC" "$asm_file" -o "$asm_output" &> "$valgrind_log"
              assembler_status=$?
              if [[ $assembler_status -eq 99 ]]; then
                echo "❌ Valgrind detected memory errors:"
                cat "$valgrind_log"
              fi
              rm "$valgrind_log"
      else
              "$ASSEMBLER_EXEC" "$asm_file" -o "$asm_output"
              assembler_status=$?
      fi

      if [[ $assembler_status -eq 99 ]]; then
        echo "❌ Valgrind reported memory issues!"
        continue
      elif [[ $assembler_status -ne 0 ]]; then
        echo "❌ Assembler failed with exit code $assembler_status"
        continue
      fi

      if [[ ! -f "$expected_output" ]]; then
        echo "❌ Expected output missing: $expected_output"
        continue
      fi

      echo "==> Running diff: diff $asm_output $expected_output"
      if diff "$asm_output" "$expected_output" > /dev/null; then
        echo "✅ Test passed for $asm_base"
      else
        echo "❌ Test failed for $asm_base"
        diff "$asm_output" "$expected_output"
      fi
    done
  fi
done

echo ""
echo "All tests finished."
