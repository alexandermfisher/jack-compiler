#!/bin/bash
set -e  # Exit on error

BUILD_DIR="build"
BUILD_TYPE="${1:-Release}"  # Default to Release if no argument given

# Usage information
if [[ "$1" == "--help" || "$1" == "-h" ]]; then
    echo "Usage: ./scripts/build.sh [BuildType] [--test]"
    echo "BuildType options:"
    echo "  Debug     - Enables debugging symbols"
    echo "  Release   - Optimized build (default)"
    echo "  MemCheck  - For Valgrind memory analysis"
    echo "--test: Runs the tests after building"
    exit 0
fi

# Clean old build
echo "🔥 Cleaning old build..."
rm -rf "$BUILD_DIR"

# Create new build directory
echo "📁 Creating new build directory..."
mkdir -p "$BUILD_DIR" && cd "$BUILD_DIR"

# Run CMake
echo "⚙️ Running CMake with build type: $BUILD_TYPE"
cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

# Compile project
echo "🚀 Building project..."
cmake --build .

echo "✅ Build complete!"

# Run tests if --test is specified
if [[ "$2" == "--test" ]]; then
    echo "🧪 Running tests..."
    ctest --output-on-failure
fi
