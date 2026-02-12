#!/bin/bash

set -e

echo "=========================================="
echo "  Cipher Application Build Script"
echo "=========================================="

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

check_dependencies() {
    echo -e "${YELLOW}Checking dependencies...${NC}"
    
    local missing=()
    
    if ! command -v cmake &> /dev/null; then
        missing+=("cmake")
    fi
    
    if ! command -v make &> /dev/null; then
        missing+=("make")
    fi

    if ! command -v gcc &> /dev/null; then
        missing+=("gcc")
    fi
    
    if ! command -v g++ &> /dev/null; then
        missing+=("g++")
    fi
    
    if ! command -v qmake6 &> /dev/null && ! command -v qmake &> /dev/null; then
        missing+=("qt6-base-dev (or qt5-default)")
    fi
    
    if [ ${#missing[@]} -ne 0 ]; then
        echo -e "${RED}Missing dependencies:${NC}"
        for dep in "${missing[@]}"; do
            echo "  - $dep"
        done
        echo ""
        echo "Install on Ubuntu/Debian:"
        echo "  sudo apt update"
        echo "  sudo apt install build-essential cmake qt6-base-dev"
        echo ""
        echo "Install on Fedora:"
        echo "  sudo dnf install gcc-c++ cmake qt6-qtbase-devel"
        echo ""
        exit 1
    fi
    
    echo -e "${GREEN}All dependencies found!${NC}"
}

build_project() {
    echo -e "${YELLOW}Building project...${NC}"
    
    rm -rf build
    mkdir -p build
    cd build
    
    echo -e "${YELLOW}Running CMake configuration...${NC}"
    cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON

    echo -e "${YELLOW}Compiling...${NC}"
    make -j$(nproc)
    
    cd ..
    
    echo -e "${GREEN}Build completed successfully!${NC}"
}

run_gui() {
    echo -e "${YELLOW}Launching GUI application...${NC}"
    
    if [ -f "build/bin/cipher_gui" ]; then
        ./build/bin/cipher_gui
    elif [ -f "build/cipher_gui" ]; then
        ./build/cipher_gui
    else
        echo -e "${RED}ERROR: GUI executable not found!${NC}"
        echo "Build may have failed. Check the output above for errors."
        exit 1
    fi
}

main() {
    check_dependencies
    build_project
    
    echo ""
    echo -e "${GREEN}=========================================="
    echo "  Build successful!"
    echo "==========================================${NC}"
    echo ""
    echo "Executables:"
    echo "  - GUI:   build/bin/cipher_gui (or build/cipher_gui)"
    echo "  - CLI:   build/bin/cipher_app (or build/cipher_app)"
    echo ""

    read -p "Do you want to run the GUI now? (y/n) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        run_gui
    fi
}

main "$@"