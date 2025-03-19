#!/bin/bash

BUILD_TYPE="release"
CLEAN=false
BIN_DIR="bin"
BINARY_NAME="hackasm"
BINARY_PATH="./build/$BUILD_TYPE/src/assembler/$BINARY_NAME"

usage() {
    echo "Usage: $0 [-b <build_type>] [-c]"
    echo "  -b <build_type> : Specify build type (default: release)"
    echo "  -c              : Clean the bin directory"
    exit 1
}

while getopts "b:c" opt; do
    case ${opt} in
        b ) BUILD_TYPE=$OPTARG ;;
        c ) CLEAN=true ;;
        * ) usage ;;
    esac
done

if [ "$CLEAN" = true ]; then
    echo "🧹 Cleaning build artifacts..."
    rm -rf "$BIN_DIR"
    echo "✅ Clean complete."
    exit 0
fi

echo "🔨 Building project with build type: $BUILD_TYPE"

# Configure if build directory doesn't exist
if [ ! -d "build/$BUILD_TYPE" ]; then
    echo "==> Running cmake preset: $BUILD_TYPE"
    cmake --preset="$BUILD_TYPE" || { echo "❌ CMake configuration failed"; exit 1; }
fi

# Run the build
echo "==> Running ninja build..."
ninja -C "build/$BUILD_TYPE" || { echo "❌ Build failed"; exit 1; }

# Copy binary to bin/
if [ -f "$BINARY_PATH" ]; then
    mkdir -p "$BIN_DIR"
    cp "$BINARY_PATH" "$BIN_DIR/"
    echo "✅ Binary copied to $BIN_DIR/$BINARY_NAME"
else
    echo "❌ Binary not found at $BINARY_PATH"
    exit 1
fi

echo "🎉 Build and copy completed successfully!"
